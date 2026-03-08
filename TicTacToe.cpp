#include "TicTacToe.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <print>
#include "Utilities/TimeUtils.h"
#include "Utilities/globalFunctions.h"
using namespace Game;
TicTacToe::TicTacToe(const char *title)
    :Engine(title)
{
    std::println("TicTacToe Constructor Called!");
}
TicTacToe::TicTacToe(int width, int height, const char* title)
    :Engine(width, height, title)
{
    std::println("TicTacToe Constructor Called!");
}

void TicTacToe::initObjects()
{
    cube = std::make_unique<Cube>(800,1000,1400);
    const auto& size = getScreenSize();
    const auto projection = glm::perspective(glm::radians(45.0f) ,
        static_cast<float>(size.x) / static_cast<float>(size.y),
        0.01f,100.0f);
    cubeShader.use();
    cubeShader.setMat4("projection",projection);
    cubeShader.setVec3("objectColor", glm::vec3{0,0,255});
}

void TicTacToe::update()
{
    using namespace ShannUtilities;
    const auto& camera = getCamera();
    // Time
    Time::currentFrame = static_cast<float>(glfwGetTime());
    Time::deltaTime = Time::currentFrame - Time::lastFrame;
    Time::lastFrame = Time::currentFrame;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

    const auto& view = camera.GetViewMatrix();
    cubeShader.use();
    cubeShader.setMat4("view", view);
}

void TicTacToe::render()
{
    const auto& size = getScreenSize();
    cubeShader.use();
    resizeObjects();
    for (auto& CubePos : Cube::CubePositions)
    {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, CubePos);

        cubeShader.setMat4("model", model);
        cube->Draw();
    }
}

void TicTacToe::resizeObjects()
{
    const auto& size = getScreenSize();
    cubeShader.use();
    glm::vec3 scaledSize(cube->size.width, cube->size.height, cube->size.depth);
    cubeShader.setVec3("aSize", scaledSize);
    glm::vec2 shaderScreenSize(static_cast<float>(size.x), static_cast<float>(size.y));
    cubeShader.setVec2("screenSize", shaderScreenSize);
}

void TicTacToe::cleanUp()
{
    cubeShader.deleteShader();
}

void TicTacToe::processInput(GLFWwindow *window)
{
    using namespace ShannUtilities;
    auto& camera = getCamera();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraUtils::Camera_Movement::FORWARD, Time::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraUtils::Camera_Movement::BACKWARD, Time::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraUtils::Camera_Movement::LEFT, Time::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraUtils::Camera_Movement::RIGHT, Time::deltaTime);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        using namespace ShannUtilities;
        if (!m_isLeftMousePressed)
        {
            const auto& size = getScreenSize();

            // 1. Get 2D Mouse Position
            const auto mousePos = getMousePosition(&getWindow());

            // 2. Convert to Normalized Device Coordinates (NDC) range [-1, 1]
            // Note: We invert the Y-axis because GLFW Y goes top-to-bottom, but OpenGL Y goes bottom-to-top
            const auto normalizedMousePos = getNormalizedVersion2D(mousePos, size);
            std::println("Mouse Pos: {}, {}", normalizedMousePos.x, normalizedMousePos.y);
            // 3. Convert to Clip Space
            const auto ray_clip = glm::vec4(normalizedMousePos.x, normalizedMousePos.y, -1.0f, 1.0f);

            // 4. Convert to View Space (using the inverse of the Projection matrix)
            glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                static_cast<float>(size.x) / static_cast<float>(size.y),
                0.01f, 100.0f);

            glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
            ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f); // Set Z to point forwards, W to 0 (it's a direction, not a point)

            // 5. Convert to World Space (using the inverse of the View matrix)
            const auto& view = camera.GetViewMatrix();
            auto ray_world = glm::vec3(glm::inverse(view) * ray_eye);

            // Normalize the vector so it has a length of exactly 1.0
            ray_world = glm::normalize(ray_world);

            // 6. Spawn the cube! Let's place it 5.0 units away along that ray
            glm::vec3 spawnPos = camera.Position + (ray_world * 5.0f);

            AddCube(spawnPos);
            std::println("Spawned cube via Raycast at: {}, {}, {}", spawnPos.x, spawnPos.y, spawnPos.z);

            m_isLeftMousePressed = true;
        }
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        m_isLeftMousePressed = false;
    }
}
