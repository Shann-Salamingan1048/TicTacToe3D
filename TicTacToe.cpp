//
// Created by USER on 1/11/2026.
//

#include "TicTacToe.h"

#include <ostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

using namespace Game;
// Force usage of high-performance GPU on Windows
void TicTacToe::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraUtils::Camera_Movement::FORWARD, time.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraUtils::Camera_Movement::BACKWARD, time.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraUtils::Camera_Movement::LEFT, time.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraUtils::Camera_Movement::RIGHT, time.deltaTime);
}
void TicTacToe::mouse_callBack(GLFWwindow* window, double xposIn, double yposIn)
{
    // 1. Retrieve the "GetStart" instance from the window
    TicTacToe* app = static_cast<TicTacToe*>(glfwGetWindowUserPointer(window));

    // Safety check: ensure the pointer isn't null
    if (!app) return;

    // 2. Now use 'app->' to access your member variables
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (app->camera.firstMouse) // Access via app->
    {
        app->camera.lastX = xpos;
        app->camera.lastY = ypos;
        app->camera.firstMouse = false;
    }

    float xoffset = xpos - app->camera.lastX; // Access via app->
    float yoffset = app->camera.lastY - ypos; // Access via app->

    app->camera.lastX = xpos;
    app->camera.lastY = ypos;

    app->camera.ProcessMouseMovement(xoffset, yoffset);
}
TicTacToe::TicTacToe(uint16_t width, uint16_t height, const char* title)
    : m_height(height), m_width(width)
{
    if (!glfwInit())
    {
        std::println("Failed to initialize GLFW");
        return;
    }
    // Updated to 4.6 to match your GLAD version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (m_window == nullptr)
    {
        std::println("Failed to create GLFW window");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwGetFramebufferSize(m_window, &m_width, &m_height);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::println("Failed to initialize GLAD");
        return;
    }
    camera.lastX = static_cast<float>(m_width) / 2.0f;
    camera.lastY = static_cast<float>(m_height) / 2.0f;

}
TicTacToe::TicTacToe(const char* title)
{
    if (!glfwInit())
    {
        std::println("Failed to initialize GLFW");
        return;
    }
    // Updated to 4.6 to match your GLAD version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWmonitor* monitor = glfwGetPrimaryMonitor(); // or a specific monitor
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    m_height = mode->height;
    m_width = mode->width;
    glfwWindowHint(GLFW_SAMPLES, 4); // Anti Aliasing to make the Line Edge Smooth
    m_window = glfwCreateWindow(m_width, m_height, title, nullptr, nullptr);
    if (m_window == nullptr)
    {
        std::println("Failed to create GLFW window");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwGetFramebufferSize(m_window, &m_width, &m_height);
    glfwSetCursorPosCallback(m_window, mouse_callBack);
    glfwSetWindowUserPointer(m_window, this);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::println("Failed to initialize GLAD");
        return;
    }

    camera.lastX = static_cast<float>(m_width) / 2.0f;
    camera.lastY = static_cast<float>(m_height) / 2.0f;

}

void TicTacToe::Start()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE); // Turn on Anti-Aliasing! // Anti Aliasing to make the Line Edge Smooth
    printCurrentUseGPU();
    Shader CubeShader(CubeVertPath,CubeFragPath);
    cube = std::make_unique<Cube>(&CubeShader);

    auto projection = glm::perspective(glm::radians(45.0f) ,
        static_cast<float>(m_width) / static_cast<float>(m_height),
        0.01f,100.0f);
    CubeShader.use();
    CubeShader.setMat4("projection",projection);
    CubeShader.setVec3("objectColor", glm::vec3{0,0,255});
    Cube::CubePositions.emplace_back( 0.0f,  0.0f,  0.0f);
    Cube::CubePositions.emplace_back( 3.0f,  0.0f,  0.0f);
    while (!isWindowRunning())
    {
        // Time
        time.currentFrame = static_cast<float>(glfwGetTime());
        time.deltaTime = time.currentFrame - time.lastFrame;
        time.lastFrame = time.currentFrame;

        processInput(m_window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        auto view = camera.GetViewMatrix();
        CubeShader.setMat4("view", view);
        for (auto& CubePos : Cube::CubePositions)
        {
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, CubePos);
            CubeShader.setMat4("model", model);
            cube->Draw();
        }
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
    CubeShader.deleteShader();
    glfwTerminate();
}

void TicTacToe::framebuffer_size_callback(GLFWwindow* window, int width, int height) noexcept
{
    glViewport(0, 0, width, height);
}


void TicTacToe::printCurrentUseGPU() noexcept
{
    // --- 6. Print System Info (Verify GPU & Version) ---
    std::println("---------------------------------------------");
    std::println("OpenGL Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    std::println("GPU Renderer:   {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER))); // Check if this says NVIDIA/AMD!
    std::println("GLAD Version:   {}.{}", GLVersion.major, GLVersion.minor);
    std::println("---------------------------------------------");
}
