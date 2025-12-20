#version 330 core

out vec4 FragColor; // Output color of the fragment

in vec2 TexCoord; // Fetched from vertex shader

uniform sampler2D myTexture; // Texture sampler (bound to texture unit 0?)

void main()
{
    FragColor = texture(myTexture, TexCoord);
}
