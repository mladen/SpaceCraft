// Color output logic

// #version 330 core
// out vec4 FragColor;

// in vec3 myCustomColor;

// void main()
// {
//     FragColor = vec4(myCustomColor, 1.0f);
// }

#version 330 core
out vec4 FragColor;

in vec3 myCustomColor;
uniform vec3 uColor;
uniform bool useUniformColor;

void main()
{
    vec3 finalColor = useUniformColor ? uColor : myCustomColor;
    FragColor = vec4(finalColor, 1.0);
}
