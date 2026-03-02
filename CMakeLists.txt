cmake_minimum_required(VERSION 3.26)
project(TicTacToe3D C CXX)

# --- 1. C++23 CONFIGURATION ---
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 2. INCLUDE DIRECTORIES ---
# This ensures both Linux and Windows can find your GLAD and header files
include_directories(${CMAKE_SOURCE_DIR}/core/include)

# --- 3. SOURCE FILES ---
set(SOURCE_FILES
        main.cpp
        core/src/glad.c
        TicTacToe.cpp
        Shader.cpp
        Texture.cpp
        stb.cpp
        vao.cpp
        vbo.cpp
        ebo.cpp
        Camera.cpp
        Shapes/Cube.cpp
        Engine/Engine.cpp
)

# --- 4. CREATE EXECUTABLE ---
add_executable(TicTacToe3D ${SOURCE_FILES})

# --- 5. ASSET COPYING (Shaders & Textures) ---
# This handles copying your folders to the build directory so the app can find them
set(ASSET_FOLDERS "glsl")

foreach(FOLDER ${ASSET_FOLDERS})
    add_custom_command(
            TARGET TicTacToe3D POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER}
            $<TARGET_FILE_DIR:TicTacToe3D>/${FOLDER}
            COMMENT "Syncing ${FOLDER} to build directory..."
    )
endforeach()

# --- 6. PLATFORM-SPECIFIC LINKING ---
if(WIN32)
    # Windows-specific libraries and local GLFW .lib
    target_link_libraries(TicTacToe3D
            PRIVATE
            "${CMAKE_SOURCE_DIR}/core/lib/glfw3.lib"
            opengl32
            gdi32
            user32
            shell32
    )
else()
    # Zorin OS / Linux native libraries
    # IMPORTANT: Run 'sudo apt install libglfw3-dev libgl1-mesa-dev' first!
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    find_package(OpenGL REQUIRED)

    target_link_libraries(TicTacToe3D
            PRIVATE
            ${GLFW_LIBRARIES}
            OpenGL::GL
            dl # Required for GLAD to load functions at runtime
    )
endif()