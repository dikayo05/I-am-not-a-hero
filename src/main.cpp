#include "Game.hpp"
#include <filesystem>
#ifdef _WIN32
#include <windows.h>
#endif

static std::filesystem::path getExecutableDirectory()
{
#ifdef _WIN32
    wchar_t buffer[MAX_PATH];
    DWORD len = GetModuleFileNameW(NULL, buffer, MAX_PATH);
    if (len == 0)
        return std::filesystem::current_path();
    return std::filesystem::path(buffer).parent_path();
#else
    return std::filesystem::current_path();
#endif
}

int main()
{
    // Ensure the working directory is the executable directory so relative
    // asset paths (e.g. "assets/..." from `include/Constants.hpp`) resolve
    // the same whether launched via installer shortcut or by double-clicking
    // the exe in File Explorer.
    std::filesystem::current_path(getExecutableDirectory());

    Game game;
    return game.run();
}