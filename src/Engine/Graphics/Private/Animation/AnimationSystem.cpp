#include "Engine/Graphics/Public/GraphicsApi.hpp"

//
#include "Engine/Time/Private/WorldClock.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Component/Private/Graphics/MeshComponent.hpp"
#include "Engine/Component/Private/Graphics/AnimatorComponent.hpp"
#include "Engine/Log/Public/LogApi.hpp"
//

// #include "Engine/ECS/Component/Graphics/MeshComponent.h"
//#include "Engine/ECS/Component/Graphics/AnimatorComponent.hpp"

#include "Engine/Graphics/Private/Animation/AnimationEval.hpp"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include <algorithm>


namespace Engine::Graphics::Animation
{
	using namespace Engine::WorldSystem::Query;
	using namespace Engine::Component;
	using namespace Engine::Time;
	using namespace Engine::Graphics::Animation;

	namespace Opt = Engine::ECS::Ops;
	namespace Time = Engine::Time;
	// namespace Anim = Engine::Graphics::Animation;

	void AnimationSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto ents = ViewWhere(ctx.rw, All<AnimatorComponent, MeshComponent>{});

		for (const auto& e : ents)
		{
			auto& anim = ctx.ww.Get<AnimatorComponent>(e);
			auto& mesh = ctx.ww.Get<MeshComponent>(e);
			
			const auto& model = mesh.modelData;

			// 基本ガード
			if (model.skeleton.bones.empty() || model.clips.empty())
			{
				Engine::Log::Write(Engine::Log::Level::Warn, "[AnimationSystem]", "Contains AnimatorComponent but clips is not set");
				continue;
			}
			if (anim.clipIndex < 0 || anim.clipIndex >= (int)model.clips.size())
			{
				Engine::Log::Write(Engine::Log::Level::Warn, "[AnimationSystem]", "AnimatorComponent's clipIndex is invalid");
				continue;
			}

			const auto& clip = model.clips[(size_t)anim.clipIndex];
			const auto& clock = ctx.rw.GetResource<WorldClockData>();

			const float dt = clock.frameDt;
			const float dur = clip.duration;


			// 早期リターン
			if (dur <= 0.0f)
			{
				SampleClip(model, clip, 0.0f, anim.localTRS, false);
				BuildPalette(model, anim.localTRS, anim.palette);
				continue;
			}

			// サブ区間の確定(clampして安全化)
			float start = std::clamp(anim.startTime, 0.0f, dur);
			float end = (anim.endTime < 0.0f) ? dur : std::clamp(anim.endTime, start, dur);//	

			// timeを進める ( start / end に依存しない純粋な積分)
			anim.time += dt * anim.speed;

			// 区間内ループ
			if (anim.loopWithinRange)
			{
				const float len = end - start;

				// start == end のときは、startに固定(動けないので)
				if (len <= 0.0f)
				{
					anim.time = start;
				}
				else
				{
					// 範囲ループ: [start, end)
					while (anim.time >= end) anim.time -= len;		// 戻す
					while (anim.time < start) anim.time += len;		// 
				}
			}
			else
			{	// loopWithinRange == falseのとき

				// 既存挙動：クリップ全体ループ or 停止だが、サブ範囲を尊重
				if (anim.loop)
				{
					// クリップ全体ループ(従来通り)
					anim.time = std::fmod(anim.time, dur);
					if (anim.time < 0.f) anim.time += dur;

					// ただし「サブ区間が指定されているなら」その範囲内に寄せる
					anim.time = std::clamp(anim.time, start, end);
				}
				else
				{
					// ループしないなら、サブ区間のendで止める
					anim.time = std::clamp(anim.time, start, end);
				}
			}

			// if (anim.loop && clip.duration > 0.f) 
			// { 
			// 	anim.time = std::fmod(anim.time, clip.duration); 
			// 	if (anim.time < 0.f) anim.time += clip.duration; 
			// }
			// else 
			// { 
			// 	anim.time = std::clamp(anim.time, 0.0f, clip.duration); 
			// }

			SampleClip(model, clip, anim.time, anim.localTRS, false);
			BuildPalette(model, anim.localTRS, anim.palette);
		}
	}
}

namespace Engine::Graphics::Animation::System
{

	//// 削除予定
	//void AnimationSystem(Engine::ECS::EntityMgr& ecs)
	//{
	//	//Opt::View<Graphic::AnimatorComponent, Graphic::MeshComponent>(ecs)

	//	for (auto e : ecs.view
	//		<Graphic::AnimatorComponent, Graphic::MeshComponent>())
	//	{
	//		auto& anim = ecs.get<Graphic::AnimatorComponent>(e);
	//		auto& mesh = ecs.get<Graphic::MeshComponent>(e);

	//		const auto& model = mesh.modelData;



	//		// 基本ガード
	//		if (model.skeleton.bones.empty() || model.clips.empty()) continue;

	//		if (anim.clipIndex < 0 || anim.clipIndex >= (int)model.clips.size()) continue;



	//		const auto& clip = model.clips[(size_t)anim.clipIndex];

	//		const auto& clock = Time::worldClock(ecs);
	//		// std::cout << "[animationSystem] deltaTime: " << clock.dt << "\n";

	//		// 時刻更新
	//		anim.time += clock.dt * anim.speed;

	//		if (anim.loop && clip.duration > 0.f)
	//		{
	//			anim.time = std::fmod(anim.time, clip.duration);
	//			if (anim.time < 0.f) anim.time += clip.duration;
	//		}
	//		else
	//		{
	//			anim.time = std::clamp(anim.time, 0.0f, clip.duration);
	//		}

	//		// クリップ評価 -> パレット
	//		Anim::SampleClip(model, clip, anim.time, anim.localTRS, false);
	//		//const auto& skel = model.skeleton;
	//		//anim.localTRS.resize(skel.bones.size());
	//		//for (size_t i = 0; i < skel.bones.size(); ++i) {
	//		//	anim.localTRS[i].t = skel.bones[i].defT;  // bind時ローカル平行移動
	//		//	anim.localTRS[i].r = skel.bones[i].defR;  // bind時ローカル回転
	//		//	anim.localTRS[i].s = skel.bones[i].defS;  // bind時ローカルスケール
	//		//}
	//		Anim::BuildPalette(model, anim.localTRS, anim.palette);
	//	}
	//}
}