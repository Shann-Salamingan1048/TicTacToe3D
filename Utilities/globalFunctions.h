#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
namespace ShannUtilities
{
    inline auto getMousePosition(GLFWwindow* window) noexcept -> glm::vec2
    {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        return glm::vec2{mouseX, mouseY};
    }
    inline auto getNormalizedVersion2D(const glm::vec2& target, const glm::i32vec2& screenSize) noexcept -> glm::vec2
    {
        const auto x = (2.0f * static_cast<float>(target.x)) / static_cast<float>(screenSize.x) - 1.0f;
        const auto y = 1.0f - (2.0f * static_cast<float>(target.y)) / static_cast<float>(screenSize.y);
        return glm::vec2{x,y};
    }

}