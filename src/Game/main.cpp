// entry point

// memo
#include <iostream>

#include "Game/Game.h"


// main
int main()
{
	// std::cout << "here\n";
	GameApp::GameApp game;
	// std::cerr << "here\n";
	// // std::cout << "[main.cpp]: Launching the game..." << std::endl;

	if (game.Initialize())
	{
		game.RunLoop();
	}

	game.Shutdown();

	// std::cout<<"hello world\n";

	return 0;

}

// Character /
// Public /
//	MovementApi.hpp
//	MovementFwd.hpp
//	CCApi.hpp
//	CCFwd.hpp
// Private /
//	Control /
//	 Movement /
//    Component /
//		IntentComponents.hpp        // ← ★統合（Facing/MovementIntent）
//		MovementRuntimeComponents.hpp（必要なら）
//	  System /
//		BuildIntent.cpp             // 旧 CharacterMovementIntentResolver.cpp
//		UpdateVelocity.cpp
//		ResolveFacing.cpp
//		System.cpp                  // 司令塔（Movement::Update）
//	CC /
//   Component /
//    CCAntiChainComponent.hpp
//   Policy /
//    CCAntiChainPolicy.hpp
//    CCAntiChainPolicyDatabase.hpp
//   System /
//    CCAntiChainSystem.cpp

// --------------- フォルダ命名規則 -----------------
// Public/ (外部API)
// Internal/ (モジュール内で共有API)
// Private/ (ドメイン実装 )



// AxisKernel
// StateHooks
// TransitionBus


// TODO:
// Internalフォルダ導入に伴う、Public→Internalフォルダへの公開ヘッダの整理
// 初期化順の自動化(依存グラフ + トポロジカルソート)
// --- FSM ---
// SkillEffectDTO{.phase = ...}と個別phaseに定義するようになっている→構文を拡張し、phaseの区間を指定したり、各phaseでの効果量を柔軟に指定できるように仕様を拡張することも考える
// 
// 