module;

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <print>

module TicTacToe;

import Camera;
import Time;
import globalFunctions;

namespace Game
{
    TicTacToe::TicTacToe(const char* title)
        : Core::Engine(title)
    {
    }

    TicTacToe::TicTacToe(int width, int height, const char* title)
        : Core::Engine(width, height, title)
    {
    }

    void TicTacToe::initObjects()
    {
        const auto& size = getScreenSize();
        const auto projection = glm::perspective(
            glm::radians(45.0f),
            static_cast<float>(size.x) / static_cast<float>(size.y),
            0.01f,
            100.0f);

        m_cubeShader.use();
        m_cubeShader.setMat4("projection", projection);
        m_cubeShader.setVec3("objectColor", glm::vec3{0.0f, 0.0f, 1.0f});
    }

    void TicTacToe::update()
    {
        using namespace ShannUtilities;
        Time::currentFrame = static_cast<float>(glfwGetTime());
        Time::deltaTime = Time::currentFrame - Time::lastFrame;
        Time::lastFrame = Time::currentFrame;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const auto& view = getCamera().GetViewMatrix();
        m_cubeShader.use();
        m_cubeShader.setMat4("view", view);
    }

    void TicTacToe::render()
    {
        m_cubeShader.use();
        resizeObjects();

        for (const auto& cubePos : Cube::CubePositions)
        {
            auto model = glm::mat4{1.0f};
            model = glm::translate(model, cubePos);

            m_cubeShader.setMat4("model", model);
            m_cube.Draw();
        }
    }

    void TicTacToe::resizeObjects() const
    {
        const auto& size = getScreenSize();

        m_cubeShader.use();
        m_cubeShader.setVec3("aSize", glm::vec3{m_cube.size.width, m_cube.size.height, m_cube.size.depth});
        m_cubeShader.setVec2("screenSize", glm::vec2{static_cast<float>(size.x), static_cast<float>(size.y)});
    }

    void TicTacToe::cleanUp()
    {
        m_cubeShader.deleteShader();
    }

    void TicTacToe::onKeyAction(int key, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            requestClose();
        }
    }

    void TicTacToe::onMouseClick(int button, int action, int mods)
    {

        if (button != GLFW_MOUSE_BUTTON_LEFT)
        {
            return;
        }

        if (action == GLFW_RELEASE)
        {
            m_mouseState = MouseClickState::None;
            return;
        }

        if (action != GLFW_PRESS || m_mouseState != MouseClickState::None)
        {
            return;
        }

        auto& camera = getCamera();
        const auto& size = getScreenSize();
        const auto mousePos = ShannUtilities::getMousePosition(&getWindow());
        const auto normalizedMousePos = ShannUtilities::getNormalizedVersion2D(mousePos, size);

        std::println("Mouse Pos: {}, {}", normalizedMousePos.x, normalizedMousePos.y);

        const auto rayClip = glm::vec4{normalizedMousePos.x, normalizedMousePos.y, -1.0f, 1.0f};
        const auto projection = glm::perspective(
            glm::radians(45.0f),
            static_cast<float>(size.x) / static_cast<float>(size.y),
            0.01f,
            100.0f);

        auto rayEye = glm::inverse(projection) * rayClip;
        rayEye = glm::vec4{rayEye.x, rayEye.y, -1.0f, 0.0f};

        const auto& view = camera.GetViewMatrix();
        auto rayWorld = glm::vec3{glm::inverse(view) * rayEye};
        rayWorld = glm::normalize(rayWorld);

        constexpr float spawnDistance = 5.0f;
        const glm::vec3 spawnPos = camera.Position + (rayWorld * spawnDistance);

        AddCube(spawnPos);
        std::println("Spawned cube via Raycast at: {}, {}, {}", spawnPos.x, spawnPos.y, spawnPos.z);

        m_mouseState = MouseClickState::LeftClick;
    }

    void TicTacToe::processInput()
    {
        auto& camera = getCamera();
        auto& window = getWindow();
        using namespace ShannUtilities;
        if (glfwGetKey(&window, GLFW_KEY_W) == GLFW_PRESS)
        {
            camera.ProcessKeyboard(CameraUtils::Camera_Movement::FORWARD, Time::deltaTime);
        }
        if (glfwGetKey(&window, GLFW_KEY_S) == GLFW_PRESS)
        {
            camera.ProcessKeyboard(CameraUtils::Camera_Movement::BACKWARD, Time::deltaTime);
        }
        if (glfwGetKey(&window, GLFW_KEY_A) == GLFW_PRESS)
        {
            camera.ProcessKeyboard(CameraUtils::Camera_Movement::LEFT, Time::deltaTime);
        }
        if (glfwGetKey(&window, GLFW_KEY_D) == GLFW_PRESS)
        {
            camera.ProcessKeyboard(CameraUtils::Camera_Movement::RIGHT, Time::deltaTime);
        }
    }
}