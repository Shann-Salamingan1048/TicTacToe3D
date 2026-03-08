#pragma once
#include "Engine/Engine.h"
#include "Shapes/Cube.h"
#include <memory>
class Cube;
namespace Game
{
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
        void resizeObjects();

    private:
        std::unique_ptr<Cube> cube;
        Shader cubeShader{CubeVertPath,CubeFragPath};
        bool m_isLeftMousePressed = false;

    };
};