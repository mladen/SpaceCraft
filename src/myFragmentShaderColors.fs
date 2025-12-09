// Color output logic

#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 ourTextureCoordinates;

uniform sampler2D texture1;

void main()
{
    vec4 tex = texture(texture1, ourTextureCoordinates);

    FragColor = tex * vec4(ourColor, 1.0); // Combine texture color with vertex color
    // FragColor = tex; // Use only the texture color
}
