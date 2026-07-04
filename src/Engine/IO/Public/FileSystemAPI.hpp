#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>
#include <optional>
#include <iostream>

namespace Engine::IO
{
	//
	// 実体パスのrootを"<shceme>://<relative-path>"に対応させる
	// res://Sounds/test.wav
	// user://Save/foo.bin
	//
	class MountTable
	{
	public:
		// Mount
		void Mount(std::string scheme, std::filesystem::path root)
		{
			mounts_[std::move(scheme)] = std::move(root);
		}

		// 
		void Unmount(std::string_view scheme)
		{
			mounts_.erase(std::string(scheme));
		}

		std::optional<std::filesystem::path> TryGetRoot(std::string_view scheme) const
		{
			auto it = mounts_.find(std::string(scheme));
			if (it == mounts_.end())
				return std::nullopt;
			return it->second;
		}

		void DebugDump(std::ostream& os) const
		{
			os << "[VFS] MoutTable:\n";
			for (const auto& [scheme, root] : mounts_)
			{
				os << " " << scheme << ":// -> " << root << "\n";
			}
		}

	private:
		// "res"  -> "C:/MyGame/Assets"
		// "user" -> "C:/MyGame/UserData"
		std::unordered_map<std::string, std::filesystem::path> mounts_;
	};

	class IPathResolver
	{
	public:
		virtual ~IPathResolver() = default;
		virtual std::optional<std::filesystem::path>
			TryResolve(std::string_view vpath) const = 0;
	};

	class VfsResolver	final : public IPathResolver
	{
	public:
		explicit VfsResolver(const MountTable& mounts) :mounts_(mounts){}

		// vpath が絶対パスならそのまま返す(ツール用途にも便利)
		std::optional<std::filesystem::path> TryResolve(std::string_view vpath) const;

	private:

		const MountTable& mounts_;

		static std::optional<std::pair<std::string_view, std::string_view>>
			SplitScheme(std::string_view vpath);
	};

	std::filesystem::path FindAncestoryDirectory
	(
		std::filesystem::path start,
		std::string_view name,
		int maxUp = 12
	);


	void InitFileSystem(::Engine::WorldSystem::Core::WorldCtx& ctx);
}