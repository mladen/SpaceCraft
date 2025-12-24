#version 330 core

out vec4 FragColor; // Output color of the fragment

// Fetched from vertex shader
in vec3 myColor;
in vec2 TexCoord;

uniform sampler2D myTexture; // Texture sampler (bound to texture unit 0?)

void main()
{
    // FragColor = texture(myTexture, TexCoord);
    // FragColor = texture(myTexture, TexCoord) * vec4(0.6, 0.2, 0.9, 0.5); // Apply 50% transparency
    // FragColor = texture(myTexture, TexCoord) * vec4(1.0, 1.0, 1.0, 0.8); // Apply 20% transparency
    // FragColor = texture(myTexture, TexCoord) * vec4(1.0, 1.0, 1.0, 1.0); // Apply 20% transparency

    // Mixing the texture color with vertex color (optional)
    FragColor = texture(myTexture, TexCoord) * vec4(myColor, 1.0);

    // FragColor = texture(myTexture, TexCoord) * vec4(1.0);
    // FragColor = texture(myTexture, TexCoord);

}
