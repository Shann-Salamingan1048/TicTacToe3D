module;

#include <cstdint>

export module TicTacToe;

import Engine;
import Cube;
import Shader;

export namespace Game
{
    enum class MouseClickState : std::uint8_t
    {
        None,
        LeftClick,
        RightClick,
        Scroll,
    };

    class TicTacToe final : public Core::Engine
    {
    public:
        explicit TicTacToe(const char* title);
        TicTacToe(int width, int height, const char* title);

    private:
        void processInput() override;
        void update() override;
        void render() override;
        void cleanUp() override;
        void initObjects() override;
        void onKeyAction(int key, int action, int mods) override;
        void onMouseClick(int button, int action, int mods) override;

        void resizeObjects() const;

    private:
        Cube m_cube{800, 1000, 1400};
        Shader m_cubeShader{CubeVertPath, CubeFragPath};
        MouseClickState m_mouseState{MouseClickState::None};
    };
}