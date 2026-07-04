#include "Engine/Platform/Pubilc/PlatformAPI.hpp"

#include <filesystem>
#include <Windows.h>

namespace Engine::Platform
{
	std::filesystem::path GetExecutableDirectory()
	{
		wchar_t buf[MAX_PATH]{};
		DWORD n = ::GetModuleFileNameW(nullptr, buf, MAX_PATH);
		std::filesystem::path exePath(std::wstring(buf, n));
		return exePath.parent_path();
	}
}