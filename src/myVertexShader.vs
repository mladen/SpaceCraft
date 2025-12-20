#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aTexCoord;

out vec2 TexCoord;

//uniform float scale; // Controls the scale of the vertices

void main()
{
    gl_Position = vec4(aPos,1.0);
    // gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0); // Outputs the positions/coordinates of all vertices

    TexCoord = aTexCoord;
}
