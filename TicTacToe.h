#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <print>
#include "TimeUtils.h"
#include "Camera.h"
#include "Shapes/Cube.h"
#include <memory>
class Camera;
class Cube;
struct Time;
namespace Game
{
    class TicTacToe
    {
    public:
        explicit TicTacToe(uint16_t width, uint16_t height, const char* title);
        explicit TicTacToe(const char* title);
        ~TicTacToe() = default;

    public:
        void Start();
    private:
        GLFWwindow* m_window;
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height) noexcept;
        bool isWindowRunning() const noexcept{ return glfwWindowShouldClose(m_window);};
        static void printCurrentUseGPU() noexcept;
        void processInput(GLFWwindow *window);
        static void mouse_callBack(GLFWwindow* window, double xpos, double ypos);

    private:
        Time time;
        CameraUtils::Camera camera{glm::vec3(0.0f, 0.0f, 3.0f)};
        std::unique_ptr<Cube> cube;
    private:
        int m_width;
        int m_height;

    };
};