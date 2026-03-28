module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <print>
#include <stdexcept>

module Engine;

namespace Core
{
    auto Engine::fromWindow(GLFWwindow* window) noexcept -> Engine*
    {
        return static_cast<Engine*>(glfwGetWindowUserPointer(window));
    }

    void Engine::framebufferSizeCallback(GLFWwindow* window, int width, int height) noexcept
    {
        glViewport(0, 0, width, height);

        if (auto* app = fromWindow(window))
        {
            app->m_windowSize = {width, height};
        }
    }

    void Engine::mouseMoveCallback(GLFWwindow* window, double xposIn, double yposIn) noexcept
    {
        auto* app = fromWindow(window);
        if (app == nullptr)
        {
            return;
        }

        const auto xpos = static_cast<float>(xposIn);
        const auto ypos = static_cast<float>(yposIn);

        if (app->m_camera.firstMouse)
        {
            app->m_camera.lastX = xpos;
            app->m_camera.lastY = ypos;
            app->m_camera.firstMouse = false;
        }

        const float xoffset = xpos - app->m_camera.lastX;
        const float yoffset = app->m_camera.lastY - ypos;

        app->m_camera.lastX = xpos;
        app->m_camera.lastY = ypos;
        app->m_camera.ProcessMouseMovement(xoffset, yoffset);
    }

    Engine::Engine(const char* title)
        : m_title{title},
          m_startFullscreen{true}
    {
        initWindow();
    }

    Engine::Engine(int width, int height, const char* title)
        : m_title{title},
          m_windowSize{width, height},
          m_startFullscreen{false}
    {
        initWindow();
    }

    auto Engine::isWindowRunning() const noexcept -> bool
    {
        return m_window != nullptr && glfwWindowShouldClose(m_window) == GLFW_FALSE;
    }

    void Engine::requestClose() noexcept
    {
        if (m_window != nullptr)
        {
            glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        }
    }

    void Engine::run()
    {
        initObjects();

        while (isWindowRunning())
        {
            glfwPollEvents();
            processInput();
            update();
            render();
            glfwSwapBuffers(m_window);
        }

        shutdown();
    }

    void Engine::initWindow()
    {
        if (glfwInit() == GLFW_FALSE)
        {
            throw std::runtime_error{"Failed to initialize GLFW."};
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if (mode == nullptr)
        {
            glfwTerminate();
            throw std::runtime_error{"Failed to query the primary monitor video mode."};
        }

        m_fullScreenSize = {mode->width, mode->height};

        const int windowWidth = m_startFullscreen ? m_fullScreenSize.x : m_windowSize.x;
        const int windowHeight = m_startFullscreen ? m_fullScreenSize.y : m_windowSize.y;

        m_window = glfwCreateWindow(windowWidth, windowHeight, m_title.c_str(), nullptr, nullptr);
        if (m_window == nullptr)
        {
            glfwTerminate();
            throw std::runtime_error{"Failed to create the GLFW window."};
        }

        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(1);
        glfwSetWindowUserPointer(m_window, this);
        glfwSetFramebufferSizeCallback(m_window, &Engine::framebufferSizeCallback);
        glfwSetCursorPosCallback(m_window, &Engine::mouseMoveCallback);
        glfwSetKeyCallback(m_window, &Engine::keyCallback);
        glfwSetMouseButtonCallback(m_window, &Engine::mouseButtonCallback);

        glfwGetFramebufferSize(m_window, &m_windowSize.x, &m_windowSize.y);

        if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
        {
            shutdown();
            throw std::runtime_error{"Failed to initialize GLAD."};
        }

        m_camera.lastX = static_cast<float>(m_windowSize.x) * 0.5f;
        m_camera.lastY = static_cast<float>(m_windowSize.y) * 0.5f;

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);

        printCurrentGpu();
    }

    void Engine::shutdown() noexcept
    {
        cleanUp();

        if (m_window != nullptr)
        {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }

        glfwTerminate();
    }

    void Engine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept
    {
        static_cast<void>(scancode);

        auto* app = fromWindow(window);
        if (app != nullptr)
        {
            app->onKeyAction(key, action, mods);
        }
    }

    void Engine::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) noexcept
    {
        auto* app = fromWindow(window);
        if (app != nullptr)
        {
            app->onMouseClick(button, action, mods);
        }
    }

    void Engine::printCurrentGpu() noexcept
    {
        std::println("---------------------------------------------");
        std::println("OpenGL Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        std::println("GPU Renderer:   {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        std::println("GLAD Version:   {}.{}", GLVersion.major, GLVersion.minor);
        std::println("---------------------------------------------");
    }
}