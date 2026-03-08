#version 460 core
layout (location = 0) in vec3 aPos;

uniform vec3 aSize;      // Your 800, 1000, 1400
uniform vec2 screenSize; // The width and height of your window

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

    // Normalize the size based on the screen.
    // X is divided by screen width.
    // Y is divided by screen height.
    // Z is divided by screen width to keep the depth scaling proportional.
    vec3 normalizedSize = vec3(2.0f * aSize.x / screenSize.x - 1.0f, 1.0f - (2.0f * aSize.y / screenSize.y), aSize.z / screenSize.x);

    // Scale the unit cube by the newly normalized fractional values
    vec3 scaledPos = aPos * normalizedSize;

    gl_Position = projection * view * model * vec4(scaledPos, 1.0);
}