cmake_minimum_required(VERSION 3.28)

project(TicTacToe3D LANGUAGES C CXX)

add_executable(TicTacToe3D)

target_compile_features(TicTacToe3D PRIVATE cxx_std_23)

set_target_properties(TicTacToe3D PROPERTIES
        CXX_EXTENSIONS OFF
)

target_sources(TicTacToe3D
        PRIVATE
        main.cpp
        core/src/glad.c
        Texture.cpp
        stb.cpp
        Camera.cpp
        Engine/Engine.cpp
        TicTacToe.cpp
        Shapes/BaseShape3D.cpp
        Shapes/Cube.cpp
        Shader.cpp
        vao.cpp
        vbo.cpp
        ebo.cpp

        PRIVATE
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
)

target_include_directories(TicTacToe3D
        PRIVATE
        ${CMAKE_SOURCE_DIR}/core/include
)

set(ASSET_FOLDERS glsl)

foreach(FOLDER IN LISTS ASSET_FOLDERS)
    add_custom_command(
            TARGET TicTacToe3D POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER}
            $<TARGET_FILE_DIR:TicTacToe3D>/${FOLDER}
            COMMENT "Syncing ${FOLDER} to build directory..."
    )
endforeach()

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

    target_include_directories(TicTacToe3D PRIVATE ${GLFW_INCLUDE_DIRS})

    target_link_libraries(TicTacToe3D
            PRIVATE
            ${GLFW_LIBRARIES}
            OpenGL::GL
            dl
    )
endif()