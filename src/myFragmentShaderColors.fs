#version 330 core
out vec4 FragColor;

in vec3 myCustomColor;

void main()
{
    FragColor = vec4(myCustomColor, 1.0f);
}