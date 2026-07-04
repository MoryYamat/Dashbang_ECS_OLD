#include "Engine/IO/Public/FileSystemAPI.hpp"

#include <filesystem>
#include <optional>


namespace Engine::IO
{
	std::optional<std::pair<std::string_view, std::string_view>>
		VfsResolver::SplitScheme(std::string_view vpath)
	{
		namespace fs = std::filesystem;

		// "res://Sounds/test.wav"
		const auto pos = vpath.find("://");
		if (pos == std::string_view::npos)
			return std::nullopt;

		const auto scheme = vpath.substr(0, pos);
		const auto rest = vpath.substr(pos + 3);		// skip "://"
		if (scheme.empty() || rest.empty())
			return std::nullopt;

		return std::make_pair(scheme, rest);
	}

	std::optional<std::filesystem::path> VfsResolver::TryResolve(std::string_view vpath) const
	{
		namespace fs = std::filesystem;

		// OS ê‚ëŒÉpÉX Ç»ÇÁÇªÇÃÇ‹Ç‹
		fs::path p(vpath);
		if (p.is_absolute())
			return fs::weakly_canonical(p);

		// scheme:// ÇÃ VFS Ç»ÇÁ mount Ç≈âåà
		const auto parts = SplitScheme(vpath);
		if (!parts)
			return std::nullopt;

		const auto [scheme, rest] = *parts;
		const auto rootOpt = mounts_.TryGetRoot(scheme);
		if (!rootOpt)
			return std::nullopt;

		fs::path restP(rest);
		if (restP.is_absolute())
			return std::nullopt;

		fs::path abs = (*rootOpt) / restP;
		return fs::weakly_canonical(abs);

	}

	std::filesystem::path FindAncestoryDirectory
	(
		std::filesystem::path start,
		std::string_view name,
		int maxUp
	)
	{
		namespace fs = std::filesystem;

		fs::path cur = fs::absolute(start);

		for (int i = 0; i <= maxUp; ++i)
		{
			fs::path candidate = cur / name;
			if (fs::exists(candidate) && fs::is_directory(candidate))
			{
				return candidate;
			}

			if (!cur.has_parent_path())
				break;

			fs::path parent = cur.parent_path();
			if (parent == cur)
				break;

			cur = std::move(parent);
		}

		return {};// å©Ç¬Ç©ÇÁÇ»Ç©Ç¡ÇΩèÍçá
	}
}