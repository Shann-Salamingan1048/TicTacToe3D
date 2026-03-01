#version 460 core

// The final color output to the screen
out vec4 FragColor;

// A color you can pass in from C++ to change the CrossHair's look
uniform vec3 objectColor;

void main()
{
    // Set the pixel to the color you chose, with 1.0 for full opacity
    FragColor = vec4(objectColor, 1.0);
}