#include "Game/Actor/Public/TestPlayerActor.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

// ------------- engine ------------- 
// component
#include "Engine/Component/Private/Common/TransformComponent.hpp"
#include "Engine/Component/Private/Logic2D/Logic2DComponent.hpp"
#include "Engine/Component/Private/Logic2D/Transform2DComponent.hpp"
#include "Engine/Component/Private/Logic2D/Velocity2DComponent.hpp"
#include "Engine/Component/Private/Logic2D/CollisionComponent.hpp"

#include "Engine/Component/Private/Graphics/ShaderComponent.hpp"
#include "Engine/Component/Private/Graphics/MeshComponent.hpp"
#include "Engine/Component/Private/Graphics/AnimatorComponent.hpp"
#include "Engine/Component/Private/Utils/NameComponent.hpp"

#include "Game/Input/Public/InputActionComponent.hpp"


// graphics
#include "Engine/Graphics/Private/Model/CgltfImporter.hpp"
#include "Engine/Graphics/Private/Model/ModelData.h"
#include "Engine/Graphics/Private/Renderer/GPUBufferUtils.h"
#include "Engine/Graphics/Public/Types.hpp"

// ------------- character ------------- 

// animation
#include "Game/Character/Animation/Public/LocomAnimComponent.hpp"
#include "Game/Character/Animation/Public/SkillAnimComponent.hpp"

// control
#include "Game/Character/Control/Public/IntentComponent.hpp"

// FSM
// movement
#include "Game/Character/FSM/Public/MovementAxisComponent.hpp"
#include "Game/Character/FSM/Public/MovementAxisApi.hpp"
#include "Game/Character/FSM/Public/SkillAxisComponent.hpp"
#include "Game/Character/FSM/Public/SkillAxisApi.hpp"

// stats
#include "Game/Character/Stats/Public/StatsComponent.hpp"

// tag
#include "Game/ECS/Public/CharacterAttribTags.h"

// ------------- Combat ------------- 
#include "Game/Combat/Skill/Internal/Core/CoreDTO.hpp"
#include "Game/Combat/Skill/Internal/Core/CoreTypes.hpp"
#include "Game/Combat/Skill/Public/CoreApi.hpp"
#include "Game/Combat/Skill/Public/SlotComponent.hpp"

// ------------- init --------------
#include "Game/Init/Private/InitModel/InitLogicTransformFromModel.h"
#include "Game/Init/Public/InitApi.hpp"

// cursor
#include "Engine/Component/Private/Tags/PlayerControllerComponent.hpp"
#include "Engine/Component/Private/Input/AnalogInputComponent.hpp"
#include "Engine/Component/Private/Input/InputBindingComponent.hpp"

#include <iostream>

namespace Game::Actor
{
	using namespace Engine::WorldSystem::Core;
	using namespace Engine::Component;
	using namespace Engine::Physics;

	namespace MFSM = Game::Character::FSM::Movement;

	namespace Ctrl = Game::Character::Control;

	using namespace Game::Character::FSM;

	using namespace Game::Combat::Skill;

	TestPlayerActor::TestPlayerActor(Engine::WorldSystem::Core::WorldCtx& ctx, Engine::Graphics::Shader* shader)
	{
		Engine::ECS::Core::Entity e = ctx.ww.Create();

		// ------- Engine --------
		// component
		Engine::Graphics::Model::ModelData mdl 
			= Engine::Graphics::Model::CgltfImporter::Import("Assets/Models/paladin/base_action_animation_diago.glb");// run_fwd_default/run_back/run_right/run_left/rolling_fwd_default
		Engine::Graphics::Model::ModelGPU mGPU = Engine::Graphics::Render::GPUBufferUtils::createMeshGPUBuffers(mdl);

		TransformComponent& tr = ctx.ww.Add<TransformComponent>(e);
		Transform2DComponent& tr2d = ctx.ww.Add<Transform2DComponent>(e);
		Logic2DTransformComponent& l2dts = ctx.ww.Add<Logic2DTransformComponent>(e);
		l2dts = Game::Init::Logic2D::InitLogic2DTransformFromModel(tr, mdl);
		Velocity2DComponent& vel = ctx.ww.Add<Velocity2DComponent>(e);

		AnimatorComponent& anim = ctx.ww.Add<AnimatorComponent>(e);

		CollisionComponent playerCollisionComp;
		playerCollisionComp.collider.shape = Circle2D{
			.center = glm::vec2(0.0f),// ローカルセンター
			.radius = Game::Init::Logic2D::EstimateRadiusFromModelXZ(tr, mdl, Game::Init::Logic2D::RadiusEstimateStrategy::MaxAxis)
		};
		playerCollisionComp.isStatic = false;
		CollisionComponent& coll = ctx.ww.Add<CollisionComponent>(e, playerCollisionComp);

		//// skill animation
		Engine::Graphics::Model::CgltfImporter::ImportAnimationsInto("Assets/Models/paladin/test_slash_outward_trim.glb", mdl);
		
		//// cc animation
		Engine::Graphics::Model::CgltfImporter::ImportAnimationsInto("Assets/Models/paladin/stunned_default.glb", mdl);
		Engine::Graphics::Model::CgltfImporter::ImportAnimationsInto("Assets/Models/paladin/falling_back_default.glb", mdl);


		// set MeshComponent (move のため最後に追加)
		MeshComponent& mesh = ctx.ww.Add<MeshComponent>(e, MeshComponent{
			 std::move(mdl),
			 std::move(mGPU)
			});


		// shader
		ShaderComponent shaderComp;
		shaderComp.shader = shader;
		if (shaderComp.shader)
		{
			shaderComp.shader->Use();
			shaderComp.shader->setMat4("model", tr.toMatrix());
			std::cout << "[PlayerCharacterActor.cpp]: The model matrix was set successfully." << std::endl;
		}
		else
		{
			std::cout << "[PlayerCharacterActor.cpp]: Shader not found." << std::endl;
		}
		ctx.ww.Add<ShaderComponent>(e, shaderComp);


		// ------------------------- Character -----------------------------

		// control
		Ctrl::MovingIntentComponent& mvint = ctx.ww.Add<Ctrl::MovingIntentComponent>(e);
		Ctrl::FacingIntentComponent& facing = ctx.ww.Add<Ctrl::FacingIntentComponent>(e);
		Ctrl::SkillIntentComponent& skillIntent = ctx.ww.Add<Ctrl::SkillIntentComponent>(e);

		// fsm
		MFSM::MovementStateComp& MFSMstate = ctx.ww.Add<MFSM::MovementStateComp>(e);
		MFSM::MovementAxisComp& MFSMComp = ctx.ww.Add<MFSM::MovementAxisComp>(e);
		auto& moveMods = ctx.ww.Add<MFSM::MovementModifierComponent>(e);
		if (!MFSM::InitMovementAxis(ctx, MFSMComp))
		{
			std::cout << "[TestActor]: Failed to initialize the movement axis component.\n";
		}
		auto& sState = ctx.ww.Add<Skill::SkillStateComp>(e);
		auto& sFSM = ctx.ww.Add<Skill::SkillAxisComp>(e);
		if (!Skill::InitSkillAxis(ctx, sFSM))
		{
			std::cout << "[TestActor]: Failed to initialize the Skill axis component.\n";
		}

		// stats
		ctx.ww.Add<Game::Character::Stats::CharacterStatsComponent>(e);

		// ---------input-----------
		ctx.ww.Add<Game::Input::InputActionComponent>(e);
		auto& input2slot = ctx.ww.Add<Game::Combat::Skill::SkillInputBindingComponent>(e);
		input2slot.DefaultInit();
		
		// ---------tag ------------
		ctx.ww.Add<Game::ECS::Tags::PlayerCharacterTag>(e);
		NameComponent& name = ctx.ww.Add<NameComponent>(e);
		name.name = "Player";

		// ------------ anim decision --------------
		ctx.ww.Add<Game::Character::Animation::FinalAnimDecisionComponent>(e);
		auto& animProf = ctx.ww.Add<Game::Character::Animation::AnimationProfileComponent>(e);
		animProf.profileId = "PaladinDefault";

		// mv
		ctx.ww.Add<Game::Character::Animation::Movement::MovementAnimDecisionComponent>(e);
		ctx.ww.Add<Game::Character::Animation::Movement::LocomotionAnimQueryComponent>(e);

		// skill
		ctx.ww.Add<::Game::Character::Animation::Skill::SkillAnimDecisionComponent>(e);
		ctx.ww.Add<::Game::Character::Animation::Skill::SkillAnimRequestComponent>(e);

		// ------------- combat ------------- 
		ctx.ww.Add<Game::Combat::Skill::SkillRuntimeComp>(e);
		auto& slot2skill = ctx.ww.Add<Game::Combat::Skill::SkillSlotComponent>(e);// slot → skill 
		SkillSlotLoadoutDTO loadout{};
		loadout.slotSkillnames = { "test_skill" };
		// 初期化処理実装必要 (SkillCatalogの登録)
		Game::Combat::Skill::InitSkillSlot(ctx, loadout, slot2skill);

		std::cout << "[Test Actor]: Test Actor Created.\n";
	}

	TestPlayerCursorActor::TestPlayerCursorActor(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Engine::ECS::Core::Entity e = ctx.ww.Create();

		ctx.ww.Add<AnalogInputComponent>(e);
		ctx.ww.Add<PlayerControllerComponent>(e);
		ctx.ww.Add<InputBindingComponent>(e);
	}
}