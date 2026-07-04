#include "Game.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std system
#include <iostream>
#include <memory>

// ======================= Engine =======================

//Entity
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

#include "Engine/WorldSystem/Private/Core/World.hpp"
#include "Engine/WorldSystem/Private/Core/WorldCtx.hpp"

#include "Engine/ECS/GlobalSystem/GlobalCleanupSystem.h"

//Components
#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

// Init Components (Must be included for specialization)
#include "Engine/ECS/Public/ECSApi.hpp"
#include "Engine/ECS/Public/InitSpecialization/FollowCamInit.hpp"

// #include "Engine/ECS/Meta/InitComponent/InitDispatcher.h"
#include "Engine/ECS/Meta/InitComponent/TileMapInit.h"
// #include "Engine/ECS/Meta/InitComponent/FollowCameraInit.h"

// Audio
#include "Engine/Audio/Public/AudioAPI.hpp"

// Window
#include "Engine/Window/Public/Window.h"

// input
#include "Engine/InputManager/Public/Types.hpp"

// physics
#include "Engine/Physics/Private/Logic2D/DetectionFunctions.h"
#include "Engine/ECS/Component/Logic2D/ColliderType.h"

// Graphic
#include "Engine/Graphics/Private/Renderer/RenderSystem.h"
#include "Engine/Graphics/Private/Model/AssimpImporter.h"

#include "Engine/Graphics/Public/GraphicsApi.hpp"

// VFX
#include "Engine/VFX/Public/VFXAPI.hpp"

// Time
#include "Engine/Time/Private/WorldClock.hpp"

// Debug
#include "Engine/Config/CanonicalDefaults.h"
#include "Engine/Debug/Public/DebugDrawAPI.hpp"

#include "Engine/Debug/Public/ProfilingAPI.hpp"

// Log
#include "Engine/Log/Public/LogApi.hpp"

// ======================= Game =======================

//Game/ Feature
#include "Game/00_Feature/Private/Character/Movement/MovementFeature.h"
//Game/ Layer
#include "Game/01_Layer/Public/LayerFeature.hpp"

//Game/Actor
//
#include "Game/Actor/Public/TestPlayerActor.hpp"
#include "Game/Actor/Public/TestCamActor.hpp"
#include "Game/Actor/Public/TestmapActor.hpp"

// Game/Actor/Map
#include "Game/Actor/Map/TileMapActor.h"
#include "Game/Actor/Map/Obstacle/TestRockActor.h"

// Game/Camera
#include "Game/Camera/Private/CameraFollowSystem.h"
#include "Game/Camera/Public/CameraApi.hpp"

// Game/Input

// Game/Input/Intent


// Game Init
#include "Game/Init/Private/InitTileMap/InitTileMap.h"
#include "Game/Init/Private/InitComponent/InputBindingInit.h"

// collision systems


// Game/Sync
#include "Engine/Sync/Private/LogicToTransformSystem.h"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Engine/IO/Public/FileSystemAPI.hpp"

#include <thread>
#include <chrono>

// コンストラクタ
GameApp::GameApp::GameApp()
	: isRunning_(true)
	, windowWidth_(1280)
	, windowHeight_(720)
{
	world_ = std::make_unique<::Engine::WorldSystem::Core::World>();
	audioSys_ = std::make_unique<::Engine::Audio::AudioSystem>();
}

GameApp::GameApp::~GameApp() = default;

// 終了処理
void GameApp::GameApp::Shutdown()
{
	unloadData();

	// Destroy all components
	// mCtx = {};

	//mECS.Clear();


	//mShader.reset();
	input_.reset();
	renderer_res_.reset();
	shader_.reset();

	audioSys_.reset();

	if(window_)window_->Shutdown();
	window_.reset();


	std::cout << "\n[Game.cpp (Shutdown)]: The application shut down successfully." << std::endl;
}

// 初期化
bool GameApp::GameApp::Initialize()
{
	

	window_ = std::make_unique<Engine::Window::Window>();
	if (!window_->Initialize(windowWidth_, windowHeight_, "Game"))
	{
		std::cerr << "[Game.cpp]: Failed to Initialize WindowManager" << std::endl;
		return false;
	}
	
	// initialize input system
	input_ = std::make_unique<Engine::Input::InputManager>(window_->GetGLFWWindow());

	// shader
	shader_ = std::make_unique<Engine::Graphics::Shader>
		(
			"shaders/basic.vertex.glsl", "shaders/basic.fragment.glsl"
		);
	
	// デバッグライン用shader
	debugLineRenderer_ = std::make_unique<Engine::Debug::DebugLineRenderer>(
		"shaders/debug_line.vertex.glsl", "shaders/debug_line.fragment.glsl"
	);

	// renderer resources
	renderer_res_ = std::make_unique<::Engine::Graphics::RendererResources>();
	renderer_res_->Init("shaders/sprite.vert.glsl", "shaders/sprite.frag.glsl");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);// 


	// ========================== World CTX ===========================
	::Engine::WorldSystem::Core::WorldCtx ctx{ *world_ };


	// AUDIO
	::Engine::Audio::InitAllAudioResourceSystem(ctx);
	bool audio = audioSys_->initialize();
	if (!audio)
	{
		::Engine::Log::Write(::Engine::Log::Level::Fatal, "AudioSystemInit", "AudioSystem initialization failed.");
	}
	// audioSys_->bind_catalog(catalog_ptr); // 必要ならInitAllAudioResourceSystemをpointerを返すように変更する

	// VFX
	::Engine::VFX::InitAllVFXResourceSystem(ctx);

	// =========================== Delayed Init =========================
	::Game::Layer::InitializeLayerFeature::DelayedInitialization(ctx);
	::Game::Layer::Debug::DebugLayerFeature::InitDebugSystem(ctx);

	::Engine::IO::InitFileSystem(ctx);

	// loadData();
	// ctxバージョン
	loadData(ctx);

	return true;
}

void GameApp::GameApp::RunLoop()
{


	while (!glfwWindowShouldClose(window_->GetGLFWWindow()) && isRunning_)
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		// mDeltaTime = currentFrame - mLastFrame;
		float deltaTime = currentFrame - lastFrame_;
		lastFrame_ = currentFrame;
		// std::cout << "[Game.cpp(DeltaTime)]: deltaTime: " << deltaTime << "\n";

		Engine::WorldSystem::Core::WorldCtx ctx{ *world_ };

		//updateGameLogics(deltaTime);
		// updateGameLogics();
		//generateOutputs();

		glfwPollEvents();

		// ctxバージョン
		Update(ctx, deltaTime);

		// profiler
		::Engine::Debug::Profiling::Profiler::EndFrame(deltaTime);
	}

	// Update();
}



void GameApp::GameApp::loadData()
{

	std::cout << "[Game.cpp]: Data loading completed successfully." << std::endl;
}

void GameApp::GameApp::unloadData()
{

}







void GameApp::GameApp::updateContext()
{
	// update RenderContext:: viewport 
	renderCtx_.viewport = glm::vec4(0, 0, window_->GetWidth(), window_->GetHeight());
}


// -- world --
// 
void GameApp::GameApp::RunInitializationPhase(Engine::WorldSystem::Core::WorldCtx& ctx)
{
	Engine::ECS::Core::Init::ApplyAllDefferedInit<
		Engine::Component::FollowCameraComponent
	>(ctx, *window_);

	// input
	Game::Init::Input::InputBindingInitializationSystem(ctx);
}

void GameApp::GameApp::spawnAllActors(Engine::WorldSystem::Core::WorldCtx& ctx)
{

	Game::Actor::TestPlayerActor testActor = Game::Actor::TestPlayerActor(ctx, shader_.get());
	//Game::Actor::TestFixedCamActor fixedCam = Game::Actor::TestFixedCamActor(ctx);
	Game::Actor::TestFollowCamActor followCam = Game::Actor::TestFollowCamActor(ctx);
	Game::Actor::TestPlayerCursorActor cursor = Game::Actor::TestPlayerCursorActor(ctx);
	Game::Actor::TestTerrainMesh terrain = Game::Actor::TestTerrainMesh(ctx, shader_.get());

	// ::Game::Layer::Debug::DebugLayerFeature::SpawnAllDummy(ctx, 1000);
}


void GameApp::GameApp::updateGameLogics(Engine::WorldSystem::Core::WorldCtx& ctx)
{
	GameApp::updateContext();
	input_->Update();
	const auto& input = input_->GetRawInput();
	if (input.keyState.count(GLFW_KEY_ESCAPE) && input.keyState.at(GLFW_KEY_ESCAPE))
	{
		isRunning_ = false;
	}

	Game::Layer::InputLayer::Update(ctx, input_->GetRawInput(), renderCtx_);

	Game::Layer::IntentLayerFeature::Update(ctx);
	
	// Debug
	//::Game::Layer::Debug::DebugLayerFeature::Update(ctx);
	
	Game::Layer::StateLayerFeature::Update(ctx);
	Game::Layer::LogicLayerFeature::Update(ctx);

}

void GameApp::GameApp::generateOutputs(Engine::WorldSystem::Core::WorldCtx& ctx)
{
	// Game::Camera::Update(ctx);

	glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Audio
	Game::Layer::AudioLayerFeature::Update(ctx, *audioSys_);

	// Engine::Graphics::UpdateRendererAll(ctx, *shader_, window_->GetAspect(), renderCtx_);
	Game::Layer::LocomotionAnimLayerFeature::Update(ctx);
	//Game::Layer::DrawLayerFeature::Update(ctx, *shader_, *window_, renderCtx_);
	Game::Layer::DrawLayerFeature::Update(ctx, *shader_, *renderer_res_, *window_, renderCtx_);

	if (debugLineRenderer_)
	{
		Game::Layer::Debug::DebugLayerFeature::UpdateDebugRender(ctx, renderCtx_, *debugLineRenderer_);
	}



	glfwSwapBuffers(window_->GetGLFWWindow());
}

void GameApp::GameApp::Update(Engine::WorldSystem::Core::WorldCtx& ctx, float realDt)
{

	Engine::Time::WorldClockSystem::BeginFrame(ctx, realDt);

	while (Engine::Time::WorldClockSystem::PopFixedStep(ctx))
	{
		updateGameLogics(ctx);
	}

	generateOutputs(ctx);

	// 
	Engine::Time::WorldClockSystem::EndFrame(ctx);
}


void GameApp::GameApp::loadData(Engine::WorldSystem::Core::WorldCtx& ctx)
{
	GameApp::spawnAllActors(ctx);

	GameApp::RunInitializationPhase(ctx);


	// ============ AudioSystem test
	// Engine::Audio::AudioCatalogResource audioRes;
	// 
	// // 登録（ロード時）
	// Engine::Audio::SoundDef def;
	// def.path = "Assets/Sounds/test.wav";
	// def.bus = Engine::Audio::AudioBus::SFX;
	// def.defaultVolume = 1.0f;
	// 
	// Engine::Audio::SoundID testId = audioRes.catalog.register_sound("Test", def);
	// 
	// // System
	// Engine::Audio::AudioSystem audioSys;
	// audioSys.bind_catalog(&audioRes.catalog);
	// audioSys.initialize();
	// 
	// // 再生
	// audioSys.play_one_shot(testId);
	// ============ AudioSystem test

	// std::this_thread::sleep_for(std::chrono::milliseconds(500));
}


// FSM関係以外の現状復帰を目指す
// 後は、Collision だけ

// Input までOK
// 次は、移動による座標更新
// カメラ追尾 OK
// 
//

// FSM: Skill / CC
// FSM: Interference
// 

// Skill Input -> Intent -> FSM -> ...系
// 初期化処理実装必要 (SkillLoadoutDTO/SkillCatalogの登録)
// 




// TODO: 
// Collision システムのデータ駆動化 / ライブラリ構造化
//