#include "TicTacToe.h"
#include <print>

#ifdef _WIN32
#include <windows.h>
    extern "C"
    {
        __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
        __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
    }
#elif defined(__linux__)
    #include <cstdlib>
    #include <unistd.h>
    #include <limits.h>

    void ensure_high_performance_gpu()
    {
            if (std::getenv("__NV_PRIME_RENDER_OFFLOAD") == nullptr)
            {
                std::println("Restarting with NVIDIA GPU...");

                // Set the environment variables for the current process space
                setenv("__NV_PRIME_RENDER_OFFLOAD", "1", 1);
                setenv("__GLX_VENDOR_LIBRARY_NAME", "nvidia", 1);

                // Get the absolute path to the current executable
                char exePath[PATH_MAX];
                ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);

                if (len != -1)
                {
                    exePath[len] = '\0';

                    // execv replaces the current process image with a new one.
                    // It inherits the environment variables we just set above.
                    char *args[] = {exePath, nullptr};
                    execv(exePath, args);

                    // If execv succeeds, this line is never reached.
                }
                std::println("Failed to restart binary for NVIDIA offloading.");
            }
    }
#endif

int main()
    {
#ifdef __linux__
        ensure_high_performance_gpu();
#endif

        Core::Engine::Start<Game::TicTacToe>("Tic Tac Toe 3D");
        return 0;
    }