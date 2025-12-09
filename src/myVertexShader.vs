// Basic position + Color attributes

#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextureCoordinates;

out vec3 ourColor;              // Outputs the color for the Fragment Shader
out vec2 ourTextureCoordinates; // Outputs the texture for the Fragment Shader

uniform float scale; // Controls the scale of the vertices

void main()
{
    // Outputs the positions/coordinates of all vertices
    gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
    // gl_Position = vec4(aPos, 1.0);

    // Assigns the colors from the Vertex Data to "ourColor"
    ourColor = aColor;
    ourTextureCoordinates = aTextureCoordinates;
}
