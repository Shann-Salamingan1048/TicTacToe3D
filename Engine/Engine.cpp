//
// Created by shann on 3/2/26.
//
#include <print>
#include "Engine.h"
using namespace Core;

void Engine::framebuffer_size_callback(GLFWwindow* window, int width, int height) noexcept
{
    glViewport(0, 0, width, height);
}
void Engine::mouse_callBack(GLFWwindow* window, double xposIn, double yposIn)
{
    // 1. Retrieve the "GetStart" instance from the window
    auto* app = static_cast<Engine*>(glfwGetWindowUserPointer(window));

    // Safety check: ensure the pointer isn't null
    if (!app) return;

    // 2. Now use 'app->' to access your member variables
    auto xpos = static_cast<float>(xposIn);
    auto ypos = static_cast<float>(yposIn);

    if (app->m_camera.firstMouse) // Access via app->
    {
        app->m_camera.lastX = xpos;
        app->m_camera.lastY = ypos;
        app->m_camera.firstMouse = false;
    }

    float xoffset = xpos - app->m_camera.lastX; // Access via app->
    float yoffset = app->m_camera.lastY - ypos; // Access via app->

    app->m_camera.lastX = xpos;
    app->m_camera.lastY = ypos;

    app->m_camera.ProcessMouseMovement(xoffset, yoffset);
}

Engine::Engine(const char *title)
    :m_Title{title}, m_isFullScreenInit(true)
{
    std::println("Engine Constructor Called!");
    std::println("Fullscreen!");
    initWindow();
}
Engine::Engine(int width, int height,const char *title)
    :m_Title{title}, m_isFullScreenInit{false}, m_Size{width,height}
{
    std::println("Engine Constructor Called!");
    std::println("Not Fullscreen!");
    initWindow();
}
void Engine::run()
{
    initObjects();
    while (!isWindowRunning())
    {
        update();

        render();
        processInput(m_window);
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
    cleanUp();
    glfwTerminate();
}
void Engine::initWindow()
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
    m_FullScreenSize.y = mode->height;
    m_FullScreenSize.x = mode->width;
    glfwWindowHint(GLFW_SAMPLES, 4); // Anti Aliasing to make the Line Edge Smooth
    if (m_isFullScreenInit)
        m_window = glfwCreateWindow(m_FullScreenSize.x, m_FullScreenSize.y, m_Title.c_str(), nullptr, nullptr);
    else
        m_window = glfwCreateWindow(m_Size.x, m_Size.y, m_Title.c_str(), nullptr, nullptr);
    if (m_window == nullptr)
    {
        std::println("Failed to create GLFW window");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwGetFramebufferSize(m_window, &m_Size.x, &m_Size.y);
    glfwSetCursorPosCallback(m_window, mouse_callBack);
    glfwSetWindowUserPointer(m_window, this);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::println("Failed to initialize GLAD");
        return;
    }
    m_camera.lastX = static_cast<float>(m_Size.x) / 2.0f;
    m_camera.lastY = static_cast<float>(m_Size.y) / 2.0f;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE); // Turn on Anti-Aliasing! // Anti Aliasing to make the Line Edge Smooth
    printCurrentUseGPU();
}

void Engine::printCurrentUseGPU() noexcept
{
    // --- 6. Print System Info (Verify GPU & Version) ---
    std::println("---------------------------------------------");
    std::println("OpenGL Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    std::println("GPU Renderer:   {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER))); // Check if this says NVIDIA/AMD!
    std::println("GLAD Version:   {}.{}", GLVersion.major, GLVersion.minor);
    std::println("---------------------------------------------");
}