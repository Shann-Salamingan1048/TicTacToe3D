module;

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <string_view>
#include <utility>

export module Engine;

import Camera;

export namespace Core
{
    class Engine
    {
    public:
        explicit Engine(const char* title);
        Engine(int width, int height, const char* title);
        virtual ~Engine() = default;

        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(Engine&&) = delete;

        template <typename T, typename... Args>
        static void Start(Args&&... args)
        {
            T instance(std::forward<Args>(args)...);
            instance.run();
        }

    protected:
        [[nodiscard]] auto getCamera() noexcept -> CameraUtils::Camera& { return m_camera; }
        [[nodiscard]] auto getCamera() const noexcept -> const CameraUtils::Camera& { return m_camera; }

        void setTitle(std::string_view title) { m_title = title; }
        void setScreenSize(glm::i32vec2 size) noexcept { m_windowSize = size; }
        void requestClose() noexcept;

        [[nodiscard]] auto isWindowRunning() const noexcept -> bool;
        [[nodiscard]] auto getWindow() const noexcept -> GLFWwindow& { return *m_window; }
        [[nodiscard]] auto getScreenSize() const noexcept -> const glm::i32vec2& { return m_windowSize; }
        [[nodiscard]] auto getFullScreenSize() const noexcept -> const glm::i32vec2& { return m_fullScreenSize; }
        [[nodiscard]] auto getTitle() const noexcept -> std::string_view { return m_title; }

        virtual void processInput() = 0;
        virtual void update() = 0;
        virtual void render() = 0;
        virtual void cleanUp() = 0;
        virtual void initObjects() = 0;
        virtual void onMouseClick(int button, int action, int mods) = 0;
        virtual void onKeyAction(int key, int action, int mods) = 0;

    private:
        void run();
        void initWindow();
        void shutdown() noexcept;

        static void framebufferSizeCallback(GLFWwindow* window, int width, int height) noexcept;
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) noexcept;
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept;
        static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) noexcept;
        static void printCurrentGpu() noexcept;
        static auto fromWindow(GLFWwindow* window) noexcept -> Engine*;

    private:
        std::string m_title;
        CameraUtils::Camera m_camera{glm::vec3(0.0f, 0.0f, 3.0f)};
        glm::i32vec2 m_windowSize{0, 0};
        glm::i32vec2 m_fullScreenSize{0, 0};
        GLFWwindow* m_window{nullptr};
        bool m_startFullscreen{false};
    };
}