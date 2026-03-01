#version 460 core

// The 3D position of each vertex coming from your C++ VBO
layout (location = 0) in vec3 aPos;

// The matrix math needed for 3D camera and perspective
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Multiply the vertex by the matrices to get its final screen position
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}