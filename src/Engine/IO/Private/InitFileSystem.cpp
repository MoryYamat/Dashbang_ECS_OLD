#include "Engine/IO/Public/FileSystemAPI.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Engine/IO/Private/VfsDiagnostics.hpp"

#include "Engine/Platform/Pubilc/PlatformAPI.hpp"

#include <iostream>

namespace Engine::IO
{
	void InitFileSystem(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& table = ctx.ww.CreateResource<MountTable>();
		
		auto exeDir = Engine::Platform::GetExecutableDirectory();
		auto assetDir = FindAncestoryDirectory(exeDir, "Assets");

		if (assetDir.empty())
		{
			std::cerr << "[VFS][ERROR] Assets directory not found from exeDir=" << exeDir << "\n";
			return;
 		}

		table.Mount("res", assetDir);

		VfsStartupCheck(
			table,
			"res://Sounds/test.wav"		// 代表ファイル
		);
	}
}