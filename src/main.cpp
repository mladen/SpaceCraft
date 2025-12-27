#include <glad/glad.h>  // For OpenGL functions
#include <GLFW/glfw3.h> // For GLFW functions (e.g., GLFWwindow, glfwCreateWindow) which help with window creation
#include <cmath>        // For math functions
#include <iostream>     // For console output
#include <vector>       // For std::vector, a dynamic array (for storing vertices, colors, etc.) which help with dynamic memory allocation

#include "shader.h"    // Include the Shader class for handling shaders
#include "stb_image.h" // Include stb_image for image loading

// Window dimensions
const int WIDTH = 1368;
const int HEIGHT = 768;

// Circle properties (currently unused, but fine)
const float RADIUS = 0.5f;
const int NUM_SEGMENTS = 100;
const float PI = 3.14159265359f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height); // Callback function for window resize
void processInput(GLFWwindow *window);                                     // Callback function for keyboard input

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW
    // Tells GLFW what version of OpenGL to use. In this case we're using OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Sets the major version of the OpenGL context to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Sets the minor version of the OpenGL context to 3

    // Tells GLFW that we're using the core profile of OpenGL
    // That means we only have access to modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use the core profile of OpenGL

    // Create a GLFW window with the specified dimensions and title
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "SpaceCraft", NULL, NULL);
    if (!window) // Checks if the window was successfully created
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);                                    // Introduces the window into the current context, making it the target for OpenGL commands
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Sets the callback function for when the window is resized

    // Initialize GLAD, which handles the OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // ENABLE BLENDING (must be AFTER context + GLAD)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // build and compile our shader program
    // ------------------------------------
    Shader myShader("../src/myVertexShader.vs", "../src/myFragmentShaderColors.fs");
    // NOTE: For triangle I'll just reuse this shader even though logically it should have its own shader!

    // Set up vertex data (and buffer(s)) and configure vertex attributes
    float triangleVertices[] = {
        // Positions            Colors
        // x, y, z              r, g, b
        // (first three values) (last three values)
        -0.9f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,    // Red, bottom left
        -0.0f, 0.5f, 0.0f, 0.1f, 0.0f, 0.0f, 1.0f, 0.0f,    // Green, bottom right
        -0.45f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f}; // Blue, top

    GLuint VAOs[2], VBOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    // TRIANGLE SETUP
    // VAO (Vertex Array Object) stores the configuration of vertex attributes and the bindings of VBOs
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s)
    glBindVertexArray(VAOs[0]);

    // VBO (Vertex Buffer Object) is used to store vertex data in GPU memory
    // Copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // SQUARE SETUP
    // Square (two triangles)
    float squareVertices[] = {
        //  positions       colors      texCoords (texture coordinates)
        0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom left
        0.9f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.25f, 0.0f, // bottom right
        0.9f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.25f, 0.25f, // top right
        0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.25f}; // top left

    unsigned int squareIndices[] = {0, 1, 2, 0, 2, 3}; // two triangles; first triangle: 0, 1, 2; second triangle: 0, 2, 3;
                                                       // this is used for EBO, which is Element Buffer Object which means we can reuse vertices

    // VAO and VBO setup for square
    // VAO is like a container for VBO and attribute settings
    // VBO is where the actual vertex data is stored
    glBindVertexArray(VAOs[1]);                                                            // Bind the VAO for the square
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);                                                // Bind the VBO for the square
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW); // Copy vertex data to VBO

    // Position attribute of square (location = 0)
    glVertexAttribPointer(
        0,                 // layout(location = 0)
        3,                 // xyz, so 3 components
        GL_FLOAT,          // type, float
        GL_FALSE,          // normalized?
        8 * sizeof(float), // STRIDE: 8 floats per vertex; 8 floats in a row in the array (3 for position, 3 for color, 2 for uv) represent a single vertex; u and v are texture coordinates
        (void *)0          // OFFSET: start at beginning
    );
    glEnableVertexAttribArray(0);

    // Color attribute of square (location = 1)
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute of square (location = 2)
    glVertexAttribPointer(
        2,                          // layout(location = 2)
        2,                          // uv, so 2 components
        GL_FLOAT,                   // type
        GL_FALSE,                   // normalized?
        8 * sizeof(float),          // STRIDE: 8 floats per vertex (3 for position, 3 for color, 2 for uv)
        (void *)(6 * sizeof(float)) // OFFSET: after the first 6 floats (in other words, skip position and color data)
    );
    glEnableVertexAttribArray(2);

    // EBO setup
    GLuint EBO;            // Element Buffer Object, which is used to store indices
    glGenBuffers(1, &EBO); // &EBO to get the address of EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);

    // TEXTURE SETUP
    GLuint texture;             // Texture variable
    glGenTextures(1, &texture); // 1 means generate 1 texture; &texture is the address of the texture variable

    glActiveTexture(GL_TEXTURE0); // Explicitly use texture unit 0, which means that when we bind the texture, it will be bound to texture unit 0
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // Flip the image vertically on load
    unsigned char *data = stbi_load("../images/minecraft_textures.jpg", &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cerr << "Failed to load texture" << std::endl;
        return -1;
    }

    int format = (nrChannels == 4 ? GL_RGBA : GL_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    // Set texture sampler ONCE (program must be active)
    myShader.use();
    myShader.setInt("myTexture", 0);

    while (!glfwWindowShouldClose(window))
    {
        // RENDER LOOP

        processInput(window); // Check for user input

        // Rendering commands
        glClearColor(0.0f, 0.875f, 1.0f, 1.0f); // Set the clear color to a nice blue color
        glClear(GL_COLOR_BUFFER_BIT);           // Clear the color buffer

        myShader.use(); // Activate the shader program

        // Triangle (note: still works, but logically should have its own shader)
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Square (textured + alpha)
        glActiveTexture(GL_TEXTURE0);          // Activate texture unit 0
        glBindTexture(GL_TEXTURE_2D, texture); // Bind the texture

        glBindVertexArray(VAOs[1]);                          // Bind the VAO for the square
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw the square using the EBO

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up resources
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height); // Sets the viewport to cover the entire window
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}