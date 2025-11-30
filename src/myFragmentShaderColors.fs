// Color output logic

// #version 330 core
// out vec4 FragColor;

// in vec3 myCustomColor;

// void main()
// {
//     FragColor = vec4(myCustomColor, 1.0f);
// }

// #version 330 core
// out vec4 FragColor;

// in vec3 myCustomColor;
// uniform vec3 uColor;
// uniform bool useUniformColor;

// void main()
// {
//     vec3 finalColor = useUniformColor ? uColor : myCustomColor;
//     FragColor = vec4(finalColor, 1.0);
// }

#version 330 core
out vec4 FragColor;
// in vec3 ourColor;
in vec3 ourPosition;

void main()
{
    FragColor = vec4(ourPosition, 1.0);    // note how the position value is linearly interpolated to get all the different colors
}