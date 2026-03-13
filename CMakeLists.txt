# --- 1. REQUIRE CMAKE 3.28 FOR MODULES ---
cmake_minimum_required(VERSION 3.28)
project(TicTacToe3D C CXX)

# --- 2. C++23 CONFIGURATION ---
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force GCC to enable module support
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fmodules-ts)
endif()

# --- 3. SOURCE FILES ---
set(SOURCE_FILES
        main.cpp
        core/src/glad.c
        Texture.cpp
        stb.cpp
)

# --- 4. CREATE EXECUTABLE ---
add_executable(TicTacToe3D ${SOURCE_FILES})

# --- 5. ATTACH C++ MODULES ---
target_sources(TicTacToe3D
        PUBLIC
        FILE_SET CXX_MODULES FILES
        Camera.cppm
        Engine/Engine.cppm
        TicTacToe.cppm
        Shapes/BaseShape3D.cppm
        Shapes/Cube.cppm
        Shader.cppm
        vao.cppm
        vbo.cppm
        ebo.cppm
        Utilities/globalFunctions.cppm
        Utilities/TimeUtils.cppm
        PRIVATE
        Camera.cpp
        Engine/Engine.cpp
        TicTacToe.cpp
        Shapes/BaseShape3D.cpp
        Shapes/Cube.cpp
        Shader.cpp
        vao.cpp
        vbo.cpp
        ebo.cpp
)

# --- 6. TARGET INCLUDE DIRECTORIES ---
# This specific line fixes the "Cannot find directory 'glad'" error in the IDE
target_include_directories(TicTacToe3D PRIVATE ${CMAKE_SOURCE_DIR}/core/include)

# --- 7. ASSET COPYING (Shaders & Textures) ---
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

# --- 8. PLATFORM-SPECIFIC LINKING ---
if(WIN32)
    target_link_libraries(TicTacToe3D
            PRIVATE
            "${CMAKE_SOURCE_DIR}/core/lib/glfw3.lib"
            opengl32
            gdi32
            user32
            shell32
    )
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    find_package(OpenGL REQUIRED)

    target_link_libraries(TicTacToe3D
            PRIVATE
            ${GLFW_LIBRARIES}
            OpenGL::GL
            dl
    )
endif()