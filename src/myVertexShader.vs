// Basic position + Color attributes

#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextureCoordinates;

out vec3 ourColor;
out vec2 ourTextureCoordinates;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
    ourTextureCoordinates = aTextureCoordinates;
}
