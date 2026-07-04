#include "AnimationEval.hpp"

#include "Engine/Graphics/Private/Model/ModelData.h"

#include <algorithm>

#include <iostream>

namespace Engine::Graphics::Animation
{
	static int findKeyL(const std::vector<float>& times, float t)
	{
		if (times.empty() || t <= times.front()) return 0;

		if (t >= times.back()) return int(times.size()) - 2;// 最後の区間

		// 二部探索も可 現在は線形探索で十分
		for (int i = 0; i + 1 < (int)times.size(); ++i)
		{
			if (t < times[i + 1]) return i;
		}

		return int(times.size()) - 2;
	}

	// 線形補間
	static glm::vec3 lerp3(const glm::vec3& a, const glm::vec3& b, float w)
	{
		return a * (1.f - w) + b * w;
	}

	void SampleClip
	(
		const Engine::Graphics::Model::ModelData& model,
		const Engine::Graphics::Model::AnimationClip& clip,
		float time,
		std::vector<TRS>& outLocalTRS,
		bool wrap
	)
	{
		const auto& skel = model.skeleton;
		outLocalTRS.assign(skel.bones.size(), TRS{});// デフォルト 0,1,1

		for (size_t i = 0; i < skel.bones.size(); ++i)
		{
			outLocalTRS[i].t = skel.bones[i].defT;
			outLocalTRS[i].r = skel.bones[i].defR;
			outLocalTRS[i].s = skel.bones[i].defS;
		}

		float t = time;
		if (wrap && clip.duration > 0.f)
		{
			t = std::fmod(time, clip.duration);
			if (t < 0) t += clip.duration;
		}
		else
		{
			t = std::clamp(time, 0.0f, clip.duration);
		}

		// 各チャンネルで上書き
		for (const auto& ch : clip.channels)
		{
			const auto& times = ch.times;
			if (times.size() <= 1)
			{
				// キー1個ならその値で上書き
				if (ch.type == Engine::Graphics::Model::ChannelType::T || ch.type == Engine::Graphics::Model::ChannelType::S)
				{
					auto v = ch.v3.empty() ? glm::vec3(0) : ch.v3[0];
					if (ch.type == Engine::Graphics::Model::ChannelType::T) outLocalTRS[ch.bone].t = v;
					else							outLocalTRS[ch.bone].s = v;
				}
				else if (ch.type == Engine::Graphics::Model::ChannelType::R && !ch.vq.empty())
				{
					outLocalTRS[ch.bone].r = glm::normalize(ch.vq[0]);
				}
				continue;
			}

			int k = findKeyL(times, t);
			float t0 = times[k], t1 = times[k + 1];// キーフレーム時刻配列
			float w = (t1 > t0) ? (t - t0) / (t1 - t0) : 0.f;// 補完係数

			if (ch.type == Engine::Graphics::Model::ChannelType::T || ch.type == Engine::Graphics::Model::ChannelType::S)
			{
				glm::vec3 v0 = ch.v3[k];
				glm::vec3 v1 = ch.v3[k + 1];
				glm::vec3 v = lerp3(v0, v1, w);
				if (ch.type == Engine::Graphics::Model::ChannelType::T) outLocalTRS[ch.bone].t = v;
				else							outLocalTRS[ch.bone].s = v;
			}
			else// rotation
			{
				glm::quat q0 = ch.vq[k];
				glm::quat q1 = ch.vq[k + 1];

				// 最適経路のための同向き補正
				if (glm::dot(q0, q1) < 0.f) q1 = -q1;
				glm::quat q = glm::slerp(q0, q1, w);// 球面線形補間
				outLocalTRS[ch.bone].r = glm::normalize(q);
			}
		}
	}

	static glm::mat4 compose(const TRS& t)
	{
		glm::mat4 T = glm::translate(glm::mat4(1), t.t);
		glm::mat4 R = glm::mat4_cast(t.r);
		glm::mat4 S = glm::scale(glm::mat4(1), t.s);
		return T * R * S;
	}

	void BuildPalette
	(
		const Engine::Graphics::Model::ModelData& model,
		const std::vector<TRS>& localTRS,
		std::vector<glm::mat4>& outPalette
	)
	{
		const auto& skel = model.skeleton;
		const size_t n = skel.bones.size();
		outPalette.resize(n);

		// --- デバッグ出力ここから ---
		// std::cout << "\n=== [Debug: BuildPalette Matrices] ===\n";
		// std::cout << "meshBindGlobal diag: "
		// 	<< model.meshBindGlobal[0][0] << ", "
		// 	<< model.meshBindGlobal[1][1] << ", "
		// 	<< model.meshBindGlobal[2][2] << "\n";
		// std::cout << "skeletonRootBindGlobal diag: "
		// 	<< model.skeletonRootBindGlobal[0][0] << ", "
		// 	<< model.skeletonRootBindGlobal[1][1] << ", "
		// 	<< model.skeletonRootBindGlobal[2][2] << "\n";

		glm::mat4 invMeshBind = glm::inverse(model.meshBindGlobal);
		glm::mat4 skelRootBind = model.skeletonRootBindGlobal;
		glm::mat4 C = invMeshBind * skelRootBind;


		// std::cout << "C diag: "
		// 	<< C[0][0] << ", "
		// 	<< C[1][1] << ", "
		// 	<< C[2][2] << "\n";
		// 
		// if (!skel.bones.empty()) {
		// 	std::cout << "invBind[0] diag: "
		// 		<< skel.bones[0].invBind[0][0] << ", "
		// 		<< skel.bones[0].invBind[1][1] << ", "
		// 		<< skel.bones[0].invBind[2][2] << "\n";
		// }
		// std::cout << "==============================\n";


		// 親子合成 (モデル空間)
		std::vector<glm::mat4> M(n, glm::mat4(1));
		for (size_t i = 0; i < n; ++i)
		{
			glm::mat4 local = compose(localTRS[i]);
			int p = skel.bones[i].parent;
			if (p >= 0) M[i] = M[(size_t)p] * local;
			else M[i] = local;
		}

		// ここで E = M * invBind を検査
		// for (int i = 0; i < std::min<int>(n, 8); ++i) {
		// 	glm::mat4 E = ( model.skeletonRootBindGlobal * M[i]) * skel.bones[i].invBind;
		// 	std::cout << "E[" << i << "] diag = "
		// 		<< E[0][0] << ", " << E[1][1] << ", " << E[2][2] << " (should be ~1)\n";
		// }


		// glm::mat4 invMeshBind = glm::inverse(model.meshBindGlobal);
		// glm::mat4 skelRootBind = model.skeletonRootBindGlobal;

		// glm::mat4 C = invMeshBind * skelRootBind;

		for (size_t i = 0; i < n; ++i)
		{
			// outPalette[i] = invMeshBind * ( skelRootBind * M[i] ) * skel.bones[i].invBind;
			outPalette[i] = (skelRootBind * M[i]) * skel.bones[i].invBind;// モデル空間における各ボーン行列
		}
	}
}