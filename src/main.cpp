#include <glad/glad.h>  // For OpenGL functions
#include <GLFW/glfw3.h> // For GLFW functions (e.g., GLFWwindow, glfwCreateWindow) which help with window creation
#include <cmath>        // For math functions
#include <iostream>     // For console output
#include <vector>       // For std::vector, a dynamic array (for storing vertices, colors, etc.) which help with dynamic memory allocation

#include "shader.h" // Include the Shader class for handling shaders

#include "stb_image.h" // Include stb_image for image loading

// Window dimensions
const int WIDTH = 1368;
const int HEIGHT = 768;

// Circle properties
const float RADIUS = 0.5f;
const int NUM_SEGMENTS = 100;
const float PI = 3.14159265359f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);                    // Callback function for window resize
void processInput(GLFWwindow *window);                                                        // Callback function for keyboard input
GLuint compileShader(GLuint type, const char *source);                                        // Function to compile a shader
GLuint createShaderProgram(const char *vertexShaderSource, const char *fragmentShaderSource); // Function to create a shader program

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

    glfwMakeContextCurrent(window);                                    // Introduces the window into the current context, which means it becomes the current OpenGL context
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Sets the callback function for when the window is resized

    // Initialize GLAD, which handles the OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader myShader("../src/myVertexShader.vs", "../src/myFragmentShaderColors.fs");
    // Shader ourShader("myVertexShader.vs", "myFragmentShaderColors.fs", "myFragmentShaderFixed.fs");

    // Set up vertex data (and buffer(s)) and configure vertex attributes
    float firstTrianglesVertices[] = {
        // First triangle
        // Positions (first 3 values) // Colors (last 3 values)
        -0.9f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // left
        -0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // right
        -0.45f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f // bottom
    };

    float secondTriangle[] = {
        // positions       // colors         // texture coords
        0.0f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // yellow
        0.9f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, // cyan
        0.45f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f  // magenta
    };

    // Set up index data; This is used to specify which vertices make up each triangle
    // GLuint indices[] = {
    //     0, 1, 2,  // first triangle; an indice of 0 refers to the first vertex (triplet of coordinates - (0.5, 0.5, 0.0) in our case)
    //     0, 2, 3}; // second triangle

    GLuint VAOs[2], VBOs[2];    // VAOs are used to store the state needed to supply vertex data to the GPU; VBOs are used to store vertex data in GPU memory
    glGenVertexArrays(2, VAOs); // we can also generate multiple VAOs or buffers at the same time
    glGenBuffers(2, VBOs);

    // FIRST TRIANGLE SETUP
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTrianglesVertices), firstTrianglesVertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // SECOND TRIANGLE SETUP (position + color + texture coordinates)
    glBindVertexArray(VAOs[1]);             // note that we bind to a different VAO now
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]); // and a different VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(
        0, // layout(location = 0)
        3, // xyz
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float), // STRIDE: 8 floats per vertex
        (void *)0          // OFFSET: start at beginning
    );
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(
        1, // layout(location = 1)
        3, // r g b
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),          // STRIDE: 6 floats again
        (void *)(3 * sizeof(float)) // OFFSET: skip xyz
    );
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // TEXTURE SETUP WOULD GO HERE
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../images/texture.jpg", &width, &height, &nrChannels, 0);

    GLuint texture;
    glGenTextures(1, &texture);            // Generate texture ID
    glActiveTexture(GL_TEXTURE0);          // Activate the texture unit first before binding texture
    glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object

    // Set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Set texture wrapping to GL_REPEAT (default wrapping method)

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // Load image data into texture
    glGenerateMipmap(GL_TEXTURE_2D);                                                          // Generate mipmaps for the texture

    stbi_image_free(data); // Free image data after generating the texture

    GLuint texture1Uni = glGetUniformLocation(myShader.ID, "texture1");
    myShader.use();              // Activate/use the shader before setting uniforms!
    glUniform1i(texture1Uni, 0); // Set it manually

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Process input
        processInput(window);

        // Rendering
        // glClearColor(0.072f, 0.13f, 0.17f, 1.0f);
        glClearColor(0.0f, 0.875f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

        // Activate shader
        myShader.use();
        // myShader.setFloat("offset", 1.0f);

        // Draw FIRST triangle using the data from the first VAO...
        myShader.setBool("useUniformColor", false);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // ...then we draw the SECOND triangle using the data from the second(!) VAO
        myShader.setBool("useUniformColor", true);
        glUniform3f(glGetUniformLocation(myShader.ID, "uColor"), 0.0f, 1.0f, 0.0f); // green
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers and poll events
        glfwSwapBuffers(window); // Swaps the front and back buffers
        glfwPollEvents();        // Polls for and processes events
    }

    // Clean up resources (Optional? Why?): de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);

    glDeleteTextures(1, &texture);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height); // Sets the viewport to cover the entire window;
                                     // It does it initially when the window is created
                                     // and whenever the window is resized!
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

GLuint compileShader(GLuint type, const char *source)
{
    // Create shader
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Check for shader compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
                  << "::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    return shader;
}

// Create and compile shader program
GLuint createShaderProgram(const char *vertexShaderSource, const char *fragmentShaderSource)
{
    // Compile shaders
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // Create shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    // Delete shaders as they're linked into the program and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}