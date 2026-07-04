#include "CgltfImporter.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <cstddef>
#include <iostream>
#include <algorithm>

namespace Engine::Graphics::Model
{
	namespace MD = Engine::Graphics::Model;

	MD::ModelData CgltfImporter::Import(const std::string& path)
	{
		MD::ModelData model;

		cgltf_options options{};
		cgltf_data* g = nullptr;
		// meta情報をパース -> cgltf_dataツリーを組立てる(meshes/primitives/attributes/accessors/bufferViews/materials...)
		if (cgltf_parse_file(&options, path.c_str(), &g) != cgltf_result_success || !g)
		{
			std::cerr << "[CgltfImporter]: Parse information creation failure\n";
			return model;
		}
		// cgltf_dataが参照しているbufferを実際に読み込んでbuffer->dataに展開する
		if (cgltf_load_buffers(&options, g, path.c_str()) != cgltf_result_success)
		{
			std::cerr << "[CgltfImporter]: Failed to read data from file\n";
			cgltf_free(g);
			return model;
		}

		//std::cout << "Meshes: " << g->meshes_count << std::endl;
		//std::cout << "Nodes: " << g->nodes_count<< std::endl;
		//std::cout << "Buffers: " << g->buffers_count << std::endl;

		for (cgltf_size mi = 0; mi < g->meshes_count; ++mi)
		{
			const cgltf_mesh& mesh = g->meshes[mi];
			for (cgltf_size pi = 0; pi < mesh.primitives_count; ++pi)
			{
				const cgltf_primitive& prim = mesh.primitives[pi];
				if (prim.type != cgltf_primitive_type_triangles) continue;

				// 属性をひろう
				const cgltf_accessor* acc_pos = nullptr;
				const cgltf_accessor* acc_nrm = nullptr;
				const cgltf_accessor* acc_uv0 = nullptr;
				const cgltf_accessor* acc_tan = nullptr;
				const cgltf_accessor* acc_joints = nullptr;
				const cgltf_accessor* acc_weights = nullptr;

				for (cgltf_size ai = 0; ai < prim.attributes_count/*5*/; ++ai)
				{
					const cgltf_attribute& a = prim.attributes[ai];
					
					auto NameOr = [](const char* s, const char* fallback) { return (s && s[0]) ? s : fallback; };

					std::cout
						<< "[gltf] mesh[" << mi << "] name='" << NameOr(mesh.name, "<no-name>") << "'"
						<< " prim[" << pi << "]"
						<< " material=" << (prim.material ? int(prim.material - g->materials) : -1)
						<< " indices=" << (prim.indices ? int(prim.indices->count) : 0)
						<< " vtx=" << (acc_pos ? int(acc_pos->count) : 0)
						<< "\n";


					switch (a.type)
					{
					case cgltf_attribute_type_position: acc_pos = a.data; break;
					case cgltf_attribute_type_normal: acc_nrm = a.data; break;
					case cgltf_attribute_type_texcoord: if (a.index == 0) acc_uv0 = a.data; break;
					case cgltf_attribute_type_tangent: acc_tan = a.data; break;
					case cgltf_attribute_type_joints: if (a.index == 0) acc_joints = a.data; break;
					case cgltf_attribute_type_weights: if (a.index == 0) acc_weights = a.data; break;
					default: break;
					}
				}
				if (!acc_pos) continue;// 位置がなければスキップ

				MD::MeshData out{};
				const cgltf_size vcount = acc_pos->count;
				out.vertices.resize(static_cast<size_t>(vcount));

				out.source_name = (mesh.name ? mesh.name : "");
				out.source_mesh_index = static_cast<int>(mi);
				out.source_prim_index = static_cast<int>(pi);

				float tmp[4] = { 0,0,0,0 };

				// 必要な値を取り出してVertexデータにコピー
				for (cgltf_size i = 0; i < vcount; ++i)
				{
					MD::VertexData v{};

					cgltf_accessor_read_float(acc_pos, i, tmp, 3);
					v.position = { tmp[0], tmp[1], tmp[2] };

					if (acc_nrm)
					{
						cgltf_accessor_read_float(acc_nrm, i, tmp, 3);
						v.normal = { tmp[0], tmp[1], tmp[2] };
					}
					else
					{
						v.normal = { 0,1,0 };
					}
					if (acc_uv0)
					{
						cgltf_accessor_read_float(acc_uv0, i, tmp, 2);
						v.texCoords = { tmp[0], tmp[1] };
					}
					else
					{
						v.texCoords = { 0,0 };
					}
					if (acc_tan && acc_nrm)
					{
						cgltf_accessor_read_float(acc_tan, i, tmp, 4);
						glm::vec3 T(tmp[0], tmp[1], tmp[2]);
						float sign = tmp[3];

						// Nに直交化(念のためGram-Schmidt)
						T = glm::normalize(T - v.normal * glm::dot(T, v.normal));
						v.tangent = T;

						// glTF規定: B = sign * normalize(cross(N, T))
						v.bitangent = glm::normalize(glm::cross(v.normal, v.tangent)) * sign;
					}
					else
					{
						v.tangent = glm::vec3(0.0f);
						v.bitangent = glm::vec3(0.0f);
					}
					if (acc_joints)
					{
						uint32_t j[4] = { 0,0,0,0 };
						cgltf_accessor_read_uint(acc_joints, i, j, 4);
						v.joints = { j[0], j[1], j[2], j[3] };
					}
					if (acc_weights)
					{
						float w[4] = { 0,0,0,0 };
						cgltf_accessor_read_float(acc_weights, i, w, 4);

						// 正規化
						float s = w[0] + w[1] + w[2] + w[3];
						if (s > 1e-8f) { for (int k = 0; k < 4; ++k) w[k] /= s; }
						else { w[0] = 1; w[1] = w[2] = w[3] = 0; }
						v.weights = { w[0],w[1],w[2],w[3] };
					}

					// 3) weaponMask（両方揃ってから）
					{
						auto inTarget = [&](uint32_t j)->bool { return j == 58; }; // まず固定
						float m = 0.0f;
						for (int k = 0; k < 4; ++k)
						{
							const uint32_t j = v.joints[k];
							const float    w = v.weights[k];
							if (w > 0.0f && inTarget(j)) m += w;
						}
						v.weaponMask = std::clamp(m, 0.0f, 1.0f);
					}

					out.vertices[(size_t)i] = v;

					model.min = glm::min(model.min, v.position);
					model.max = glm::max(model.max, v.position);
				}

				// index
				if (prim.indices)
				{
					out.indices.resize((size_t)prim.indices->count);
					for (cgltf_size i = 0; i < prim.indices->count; ++i)
					{
						cgltf_size v = cgltf_accessor_read_index(prim.indices, i);
						out.indices[(size_t)i] = (unsigned int)v;
					}
					out.hasIndices = true;
				}
				else
				{
					out.indices.resize((size_t)vcount);
					for (cgltf_size i = 0; i < vcount; ++i)
					{
						out.indices[(size_t)i] = (unsigned int)i;
						out.hasIndices = true;
					}
				}

				// 色だけ
				if (prim.material)
				{
					const auto& pbr = prim.material->pbr_metallic_roughness;
					
					// 既存: ベースカラー係数
					out.materialData.baseColor =
					{
						pbr.base_color_factor[0], pbr.base_color_factor[1], pbr.base_color_factor[2]
					};

					if (pbr.base_color_texture.texture && pbr.base_color_texture.texture->image)
					{
						const cgltf_image* img = pbr.base_color_texture.texture->image;

						if (img->buffer_view)
						{
							const unsigned char* bytes = nullptr;
							size_t sz = 0;

							if (GetImageBytes_FromBufferView(img, g, bytes, sz))
							{
								GLuint id = CreateGLTexture2D_FromMemory_sRGB(bytes, sz);
								if (id != 0)
								{
									MD::TextureData tex{};
									tex.id = id;
									tex.type = "diffuse";
									tex.path = "";
									out.materialData.textures.push_back(std::move(tex));
								}
							}

							//if(GetImageBytes_FromBufferView(img, g, bytes, sz))
							//{
							//	std::cout << "[CgltfImporter]: embedded baseColorTexture bytes = "
							//		<< sz << " bytes\n";
							//}
							//else
							//{
							//	std::cout << "[CgltfImporter]: no bufferView image for baseColorTexture\n";
							//}
						}

					}
				}

				unsigned int maxIdx = 0;
				for (auto idx : out.indices) maxIdx = std::max(maxIdx, idx);

				std::cout << "[meshIdx] name='" << out.source_name
					<< "' vtx=" << out.vertices.size()
					<< " idx=" << out.indices.size()
					<< " maxIdx=" << maxIdx
					<< "\n";



				glm::vec3 mn(FLT_MAX), mx(-FLT_MAX);
				for (auto& v : out.vertices) { mn = glm::min(mn, v.position); mx = glm::max(mx, v.position); }

				std::cout << "[meshAABB] name='" << out.source_name
					<< "' vtx=" << out.vertices.size()
					<< " idx=" << out.indices.size()
					<< " min=(" << mn.x << "," << mn.y << "," << mn.z << ")"
					<< " max=(" << mx.x << "," << mx.y << "," << mx.z << ")"
					<< "\n";

				model.meshes.emplace_back(std::move(out));
			}
		}

		// skelton
		if (g->skins_count > 0)
		{
			const cgltf_skin* skin = &g->skins[0];
			Skeleton& skel = model.skeleton;
			skel.bones.resize(skin->joints_count);

			// 逆引きテーブル
			for (cgltf_size bi = 0; bi < skin->joints_count; ++bi)
			{
				const cgltf_node* jn = skin->joints[bi];
				Bone b;
				b.nodeIndex = int(jn - g->nodes);

				if (jn->name)// ボーン名
				{
					b.name = jn->name;
					skel.nameToBone[b.name] = static_cast<int>(bi);// ボーン名 -> indexのマッピング
				}

				// inverseBindMatrix
				if (skin->inverse_bind_matrices)
				{
					float m[16];
					cgltf_accessor_read_float(skin->inverse_bind_matrices, bi, m, 16);
					b.invBind = glm::make_mat4(m);
				}

				// デフォルトTRS
				if (jn->has_translation) b.defT = glm::vec3(jn->translation[0], jn->translation[1], jn->translation[2]);
				if (jn->has_rotation) b.defR = glm::normalize(glm::quat(jn->rotation[3], jn->rotation[0], jn->rotation[1],jn->rotation[2]));// うまくいかない場合この順序が違う可能性あり
				if (jn->has_scale) b.defS = glm::vec3(jn->scale[0], jn->scale[1], jn->scale[2]);

				//std::cout << "[dbg] rotationQuat(xyz w)=("
				//	<< jn->rotation[0] << ", "
				//	<< jn->rotation[1] << ", "
				//	<< jn->rotation[2] << ", "
				//	<< jn->rotation[3] << ")\n";


				skel.bones[bi] = b;
				skel.nodeToBone[b.nodeIndex] = int(bi);
			}

			// 親子関係を構築
			std::vector<int> nodeParent(g->nodes_count, -1);
			for (cgltf_size ni = 0; ni < g->nodes_count; ++ni)
			{
				const cgltf_node* n = &g->nodes[ni];
				for (cgltf_size ci = 0; ci < n->children_count; ++ci)
				{
					int c = int(n->children[ci] - g->nodes);
					nodeParent[c] = int(ni);
				}
			}
			for (size_t bi = 0; bi < skel.bones.size(); ++bi)
			{
				int pNode = nodeParent[skel.bones[bi].nodeIndex];
				auto it = skel.nodeToBone.find(pNode);
				skel.bones[bi].parent = (it == skel.nodeToBone.end()) ? -1 : it->second;
			}
		}

		// アニメーション
		for (cgltf_size ai = 0; ai < g->animations_count; ++ai)
		{
			const cgltf_animation* a = &g->animations[ai];

			AnimationClip clip;
			clip.name = a->name ? a->name : ("anim_" + std::to_string(ai));

			for (cgltf_size ci = 0; ci < a->channels_count; ++ci)
			{
				const cgltf_animation_channel& ch = a->channels[ci];
				const cgltf_animation_sampler& sp = *ch.sampler;

				// 対象ノードがボーンかどうか
				int nodeIndex = int(ch.target_node - g->nodes);
				auto it = model.skeleton.nodeToBone.find(nodeIndex);
				if (it == model.skeleton.nodeToBone.end()) continue;
				int bone = it->second;

				Channel c{};
				c.bone = bone;

				if (ch.target_path == cgltf_animation_path_type_translation) c.type = ChannelType::T;
				else if (ch.target_path == cgltf_animation_path_type_rotation) c.type = ChannelType::R;
				else if (ch.target_path == cgltf_animation_path_type_scale) c.type = ChannelType::S;
				else continue;// weights などは今回無視

				// 時間
				c.times.resize(sp.input->count);
				for (cgltf_size i = 0; i < sp.input->count; ++i)
				{
					float t;
					cgltf_accessor_read_float(sp.input, i, &t, 1);
					c.times[i] = t;
					clip.duration = std::max(clip.duration, t);
				}

				// 値
				if (c.type == ChannelType::T || c.type == ChannelType::S)
				{
					c.v3.resize(sp.output->count);
					for (cgltf_size i = 0; i < sp.output->count; ++i)
					{
						float v[3];
						cgltf_accessor_read_float(sp.output, i, v, 3);
						c.v3[i] = glm::vec3(v[0], v[1], v[2]);
					}
				}
				else if (c.type == ChannelType::R)
				{
					c.vq.resize(sp.output->count);
					for (cgltf_size i = 0; i < sp.output->count; ++i)
					{
						float q[4];
						cgltf_accessor_read_float(sp.output, i, q, 4);
						c.vq[i] = glm::normalize(glm::quat(q[3], q[0], q[1], q[2])); // (w, x, y, z) // うまくいかない場合この順番を見直す
					}
				}

				clip.channels.push_back(std::move(c));
			}

			model.clips.push_back(std::move(clip));
		}

		std::cerr << "Clips: " << model.clips.size() << std::endl;
		for (auto& c : model.clips) {
			std::cerr << "  " << c.name << " dur=" << c.duration << "s channels=" << c.channels.size() << "\n";
		}

		// std::cout << "[dbg] bones=" << model.skeleton.bones.size() << "\n";
		for (size_t i = 0;i < 3 && i < model.skeleton.bones.size();++i) {
			auto& b = model.skeleton.bones[i];
			// だいたい単位長？
			//std::cout << "[dbg] bone[" << i << "] parent=" << b.parent
			//	<< " | |defR|=" << glm::length(glm::vec4(b.defR.x, b.defR.y, b.defR.z, b.defR.w))
			//	<< " | invBind[0][0]=" << b.invBind[0][0] << "\n";
		}

		std::vector<int> parent(g->nodes_count, -1);
		for (cgltf_size i = 0; i < g->nodes_count; ++i)
		{
			if (g->nodes[i].parent) parent[i] = int(g->nodes[i].parent - g->nodes);
		}

		std::vector <glm::mat4> globals;
		BuildNodeGlobals(g, parent, globals);

		int meshNodeIndex = -1;
		if (g->skins_count > 0) {
			const cgltf_skin* skin0 = &g->skins[0];
			for (cgltf_size ni = 0; ni < g->nodes_count; ++ni) {
				const cgltf_node* n = &g->nodes[ni];
				if (n->mesh && n->skin == skin0) { // glTFの仕様：スキンはノードに付く
					meshNodeIndex = int(ni);
					break;
				}
			}
		}
		if (meshNodeIndex < 0) {
			for (cgltf_size ni = 0; ni < g->nodes_count; ++ni) {
				if (g->nodes[ni].mesh) { meshNodeIndex = int(ni); break; }
			}
		}
		model.meshBindGlobal = (meshNodeIndex >= 0) ? globals[meshNodeIndex] : glm::mat4(1.0f);

		// --- set skeletonRootBindGlobal -----------------------------------------
		// skin->skeleton があればそれ。無ければ、任意ジョイントから親を辿って最上位をルートにする。
		// スキンが無ければ単位行列。
		glm::mat4 skelRoot = glm::mat4(1.0f);
		if (g->skins_count > 0) {
			const cgltf_skin* skin0 = &g->skins[0];
			if (skin0->skeleton) {
				int idx = int(skin0->skeleton - g->nodes);
				skelRoot = globals[idx];
			}
			else {
				// 最上位ジョイントを見つける
				int any = int(skin0->joints[0] - g->nodes);
				while (parent[any] >= 0) any = parent[any];
				skelRoot = globals[any];
			}
		}
		model.skeletonRootBindGlobal = skelRoot;

		// for (size_t i = 0; i < model.meshes.size(); ++i)
		// {
		// 	std::cout << "[model] i=" << i
		// 		<< " name='" << model.meshes[i].source_name << "'"
		// 		<< " mi=" << model.meshes[i].source_mesh_index
		// 		<< " pi=" << model.meshes[i].source_prim_index
		// 		<< "\n";
		// }

		// ロード直後に1回だけ
		std::cout << "[bones] count=" << model.skeleton.bones.size() << "\n";
		int n = 0;
		for (const auto& [name, idx] : model.skeleton.nameToBone)
		{
			std::cout << "  [" << n++ << "] " << idx << " : " << name << "\n";
		}


		cgltf_free(g);
		return model;
	}

	// img.buffer_view が指す生データ領域(PNG/JPGそのもの)をbytes/sizeに返す
	bool CgltfImporter::GetImageBytes_FromBufferView(
		const cgltf_image* img,// memory上にあり
		const cgltf_data* g,
		const unsigned char*& bytes,
		size_t& size
	)
	{
		bytes = nullptr;
		size = 0;

		if (!img || !img->buffer_view) return false;

		const cgltf_buffer_view* bv = img->buffer_view;
		if (!bv || !bv->buffer || !bv->buffer->data) return false;

		// offset境界チェック
		const size_t buf_size = static_cast<size_t>(bv->buffer->size);
		const size_t off = static_cast<size_t>(bv->offset);
		const size_t len = static_cast<size_t>(bv->size);
		if (off > buf_size || len > buf_size - off) return false;

		const unsigned char* base =
			static_cast<const unsigned char*>(bv->buffer->data);

		bytes = base + off;
		size = len;
		return (bytes && size > 0);
	}

	// 
	GLuint CgltfImporter::CreateGLTexture2D_FromMemory_sRGB(
		const unsigned char* bytes,
		size_t size
	)
	{
		if (!bytes || size == 0) return 0;

		int w = 0, h = 0, comp = 0;

		// glTFは通常上下反転不要．もし逆なら stbi_set_flip\vertically_on_load(true) を検討
		// comp: 元のチャンネル数 
		stbi_uc* rgba = stbi_load_from_memory(bytes, static_cast<int>(size), &w, &h, &comp, 4);
		if (!rgba) {
			std::cerr << "[CgltfImporter]: stb fail: " << stbi_failure_reason() << "\n";
			return 0;
		}

		GLuint tex = 0;
		// OpenGL テクスチャ生成
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);

		// unpack (行サイズ境界の罠回避)
		GLint prevUnpack = 4;
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &prevUnpack);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		
		// sRGBの内部フォーマット（カラー用）
		GLint internalFormat = GL_SRGB8_ALPHA8;// フォールバックが必要なら GL_RGBA8 に変更
		// GPUへ画素転送
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);


		// サンプラ状態		
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// ラップ/フィルタは glTF の sampler を後で反映 今はデフォルトで
		glPixelStorei(GL_UNPACK_ALIGNMENT, prevUnpack);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(rgba);// free


		return tex;// -> TextureData.id になる
	}


	glm::mat4 CgltfImporter::LocalOf(const cgltf_node* n)
	{
		if (n->has_matrix)
			return glm::make_mat4(n->matrix);

		glm::vec3 T = n->has_translation ? glm::vec3(n->translation[0], n->translation[1], n->translation[2]) : glm::vec3(0);
		glm::quat R = n->has_rotation ? glm::quat(n->rotation[3], n->rotation[0], n->rotation[1], n->rotation[2]) : glm::quat(1, 0, 0, 0);
		glm::vec3 S = n->has_scale ? glm::vec3(n->scale[0], n->scale[1], n->scale[2]) : glm::vec3(1);
		return glm::translate(glm::mat4(1), T) * glm::mat4_cast(R) * glm::scale(glm::mat4(1), S);
	}

	void CgltfImporter::BuildNodeGlobals(
		const cgltf_data* g,
		std::vector<int>& parent,
		std::vector<glm::mat4>& globals
	)
	{
		globals.assign(g->nodes_count, glm::mat4(1));
		std::function<void(int)> dfs = [&](int idx)
			{
				const cgltf_node* n = &g->nodes[idx];
				int p = parent[idx];
				globals[idx] = (p >= 0 ? globals[p] : glm::mat4(1)) * LocalOf(n);
				for (cgltf_size ci = 0; ci < n->children_count; ++ci)
				{
					int child_index = int(n->children[ci] - g->nodes);
					dfs(child_index);
				}
					
			};

		for (cgltf_size ni = 0; ni < g->nodes_count; ++ni)
		{
			if (parent[ni] < 0) dfs(int(ni));
		}
	}


	// アニメーションだけ読み込んで base に追加
	void CgltfImporter::ImportAnimationsInto(const std::string& path, MD::ModelData& base)
	{
		cgltf_options opt{};
		cgltf_data* g = nullptr;
		if (cgltf_parse_file(&opt, path.c_str(), &g) != cgltf_result_success || !g)
		{
			std::cerr << "[CgltfImproter] anim-only parse failed: " << path << "\n";
			return;
		}
		if (cgltf_load_buffers(&opt, g, path.c_str()) != cgltf_result_success)
		{
			std::cerr << "[CgltfImporter] anim-only load buffers failed: " << path << "\n";
			cgltf_free(g);
			return;
		}

		const auto& nameToBone = base.skeleton.nameToBone;
		if (nameToBone.empty())
		{
			std::cerr << "[CgltfImporter] base skelton has no name map. load base first. \n";
			cgltf_free(g);
			return;
		}

		for (cgltf_size ai = 0; ai < g->animations_count; ++ai)
		{
			const cgltf_animation* a = &g->animations[ai];

			MD::AnimationClip clip;
			clip.name = a->name ? a->name : ("anim_" + std::to_string(ai));
			clip.duration = 0.0f;

			for (cgltf_size ci = 0; ci < a->channels_count; ++ci)
			{
				const cgltf_animation_channel& ch = a->channels[ci];
				const cgltf_animation_sampler& sp = *ch.sampler;

				// node名でベース骨を引く
				const cgltf_node* tn = ch.target_node;
				if (!tn || !tn->name)continue;
				auto it = nameToBone.find(tn->name);
				if (it == nameToBone.end())
				{
					std::cerr << "[anim-map] missing bone: " << tn->name << "\n";
					continue;
				}
				int bone = it->second;

				MD::Channel c{};
				c.bone = bone;
				if (ch.target_path == cgltf_animation_path_type_translation) c.type = MD::ChannelType::T;
				else if (ch.target_path == cgltf_animation_path_type_rotation) c.type = MD::ChannelType::R;
				else if (ch.target_path == cgltf_animation_path_type_scale) c.type = MD::ChannelType::S;
				else continue;

				c.times.resize(sp.input->count);
				for (cgltf_size i = 0; i < sp.input->count; ++i)
				{
					float t;
					cgltf_accessor_read_float(sp.input, i, &t, 1);
					c.times[i] = t;
					if (t > clip.duration) clip.duration = t;
				}

				// 値
				if (c.type == MD::ChannelType::R)
				{
					c.vq.resize(sp.output->count);
					for (cgltf_size i = 0; i < sp.output->count; ++i)
					{
						float q[4];
						cgltf_accessor_read_float(sp.output, i, q, 4);
						c.vq[i] = glm::normalize(glm::quat(q[3], q[0], q[1], q[2]));
					}
				}
				else
				{
					c.v3.resize(sp.output->count);
					for (cgltf_size i = 0; i < sp.output->count; ++i)
					{
						float v[3];
						cgltf_accessor_read_float(sp.output, i, v, 3);
						c.v3[i] = glm::vec3(v[0], v[1], v[2]);
					}
				}

				clip.channels.push_back(std::move(c));
			}

			std::cerr << "[ImportAnimationsInto] add clip = " << clip.name
				<< " ch=" << clip.channels.size()
				<< " dur=" << clip.duration << "s\n";

			if (!clip.channels.empty())
				base.clips.push_back(std::move(clip));
		}

		cgltf_free(g);
	}
}