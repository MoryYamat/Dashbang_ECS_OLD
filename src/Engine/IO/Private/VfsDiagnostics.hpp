// Engine/IO/Public/VfsDiagnostics.hpp
#pragma once
#include "Engine/IO/Public/FileSystemAPI.hpp"
#include <filesystem>
#include <iostream>
#include <string_view>

namespace Engine::IO
{
	inline void VfsStartupCheck(
		const MountTable& mounts,
		std::string_view sentinelVPath,   // 例: "res://Sounds/test.wav"
		std::ostream& os = std::cerr
	)
	{
		// 1) いまの mount 状況を必ず出す
		mounts.DebugDump(os);

		// 2) sentinel を解決して、実体パスと exists を出す
		VfsResolver resolver(mounts);

		const auto absOpt = resolver.TryResolve(sentinelVPath);
		if (!absOpt)
		{
			os << "[VFS][ERROR] Resolve failed: " << sentinelVPath << "\n";
			os << "  (scheme not mounted? or malformed vpath?)\n";
			return;
		}

		const std::filesystem::path& abs = *absOpt;
		const bool exists = std::filesystem::exists(abs);
		os << "[VFS] Sentinel: " << sentinelVPath << "\n";
		os << "  -> " << abs << "\n";
		os << "  exists=" << (exists ? "true" : "false") << "\n";

		if (!exists)
		{
			os << "[VFS][ERROR] Sentinel file not found.\n";
			os << "  Likely causes:\n";
			os << "  - Assets not copied to the build/output directory\n";
			os << "  - CMake was not re-configured after changing Assets\n";
			os << "  - You are mounting a different Assets directory than you expect\n";
		}
	}
}
