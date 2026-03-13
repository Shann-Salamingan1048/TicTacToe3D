module;

// --- GLOBAL MODULE FRAGMENT ---
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <utility>
#include <string>
#include <string_view>

// --- MODULE DECLARATION ---
export module Engine;

// --- IMPORTS ---
import Camera;

export namespace Core
{
    class Engine
    {
    public:
        explicit Engine(int width, int height, const char* title);
        explicit Engine(const char* title);

        virtual ~Engine() = default;

        // Templates stay perfectly intact inside the export module!
        template<typename T, typename... Args>
        static void Start(Args&&... args)
        {
            T instance(std::forward<Args>(args)...);
            instance.run();
        }

    private:
        void run();
        void initWindow();

    private:
        std::string m_Title;
        CameraUtils::Camera m_camera{glm::vec3(0.0f, 0.0f, 3.0f)};
        glm::i32vec2 m_Size{0,0};
        glm::i32vec2 m_FullScreenSize{0,0};
        bool m_isFullScreenInit{false};

    protected:
        auto getCamera() noexcept -> CameraUtils::Camera&
        {
            return m_camera;
        }

    protected:
        void setTitleStr(std::string_view title) noexcept
        {
            m_Title = title;
        }

        void setScreenHeight(int height) noexcept
        {
            m_Size.y = height;
        }

        void setScreenWidth(int width) noexcept
        {
            m_Size.x = width;
        }

        void setScreenSize(glm::i32vec2 size) noexcept
        {
            m_Size = size;
        }

    protected:
        auto isWindowRunning() const noexcept -> bool
        {
            return glfwWindowShouldClose(m_window);
        }

        auto getWindow() const noexcept -> GLFWwindow&
        {
            return *m_window;
        }

        auto getScreenSize() const noexcept -> const glm::i32vec2&
        {
            return m_Size;
        }

        auto getFullScreenSize() const noexcept -> glm::i32vec2
        {
            return m_FullScreenSize;
        }

        auto getTitle() const noexcept -> std::string_view
        {
            return m_Title;
        }

    protected:
        virtual void processInput(GLFWwindow *window) = 0;
        virtual void update() = 0;
        virtual void render() = 0;
        virtual void cleanUp() = 0;
        virtual void initObjects() = 0;
        virtual void onMouseClick(int button, int action, int mods) = 0;
        virtual void onKeyAction(int button, int action, int mods, GLFWwindow* window) = 0;

    private:
        GLFWwindow* m_window = nullptr;

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height) noexcept;
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mod) noexcept;
        static void printCurrentUseGPU() noexcept;
        static void mouse_callBack(GLFWwindow* window, double xpos, double ypos);
    };
}