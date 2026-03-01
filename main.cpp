#include "TicTacToe.h"

#ifdef _WIN32
#include <windows.h>
    extern "C"
    {
        // These tell the Windows drivers to use the dedicated GPU by default
        __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
        __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
    }
#elif defined(__linux__)
    void ensure_high_performance_gpu()
{
        if (std::getenv("__NV_PRIME_RENDER_OFFLOAD") == nullptr)
        {
            std::println("Restarting with NVIDIA GPU...");
            setenv("__NV_PRIME_RENDER_OFFLOAD", "1", 1);
            setenv("__GLX_VENDOR_LIBRARY_NAME", "nvidia", 1);
            // This part is messy: you have to re-execute your own binary
        }
    }
#endif
int main()
{
#ifdef __linux__
    ensure_high_performance_gpu();
#endif
    Game::TicTacToe ttt("Tic Tac Toe 3D");
    ttt.Start();
    return 0;
}