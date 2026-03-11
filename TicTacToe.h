#pragma once
#include "Engine/Engine.h"
#include "Shapes/Cube.h"
#include <memory>
class Cube;

namespace Game
{
    enum class MouseClickState : uint8_t
    {
        LEFT_CLICK,
        RIGHT_CLICK,
        SCROLL,
        NONE
    };
    class TicTacToe : public Core::Engine
    {
    public:
        explicit TicTacToe(int width, int height, const char* title);
        explicit TicTacToe(const char* title);

    private:
        void processInput(GLFWwindow *window) override;
        void update() override;
        void render() override;
        void cleanUp() override;
        void initObjects() override;
    private:
        void onKeyAction(int button, int action, int mods,GLFWwindow* window) override;
        void onMouseClick(int button, int action, int mods) override;
    private:
        void resizeObjects() const;

    private:
        Cube cube{800,1000,1400};
        Shader cubeShader{CubeVertPath,CubeFragPath};
        MouseClickState mouseState = MouseClickState::NONE;

    };
};