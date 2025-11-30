#include <glad/glad.h>  // For OpenGL functions
#include <GLFW/glfw3.h> // For GLFW functions (e.g., GLFWwindow, glfwCreateWindow) which help with window creation
#include <cmath>        // For math functions
#include <iostream>     // For console output
#include <vector>       // For std::vector, a dynamic array (for storing vertices, colors, etc.) which help with dynamic memory allocation

#include "shader.h" // Include the Shader class for handling shaders

// Window dimensions
const int WIDTH = 1368;
const int HEIGHT = 768;

// Circle properties
const float RADIUS = 0.5f;
const int NUM_SEGMENTS = 100;
const float PI = 3.14159265359f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);                          // Callback function for window resize
void processInput(GLFWwindow *window);                                                              // Callback function for keyboard input
unsigned int compileShader(unsigned int type, const char *source);                                  // Function to compile a shader
unsigned int createShaderProgram(const char *vertexShaderSource, const char *fragmentShaderSource); // Function to create a shader program

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

    // Moves the triangle right 0.5 units in the x direction
    float offset = 0.7f;

    // Set up vertex data (and buffer(s)) and configure vertex attributes
    float firstTrianglesVertices[] = {
        // First triangle
        // Positions (first 3 values) // Colors (last 3 values)
        -0.9f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // left
        -0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // right
        -0.45f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f // bottom
    };

    float secondTriangle[] = {
        // Second triangle
        0.0f, -0.5f, 0.0f, // left
        0.9f, -0.5f, 0.0f, // right
        0.45f, 0.5f, 0.0f  // top
    };

    // float secondTriangle[] = {
    //     // positions         // colors
    //     0.0f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // yellow
    //     0.9f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, // cyan
    //     0.45f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f  // magenta
    // };

    // Set up index data; This is used to specify which vertices make up each triangle
    // unsigned int indices[] = {
    //     0, 1, 2,  // first triangle; an indice of 0 refers to the first vertex (triplet of coordinates - (0.5, 0.5, 0.0) in our case)
    //     0, 2, 3}; // second triangle

    unsigned int VBOs[2], VAOs[2];
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

    // SECOND TRIANGLE SETUP
    glBindVertexArray(VAOs[1]);             // note that we bind to a different VAO now
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]); // and a different VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0); // because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Note that this is allowed, the call to glVertexAttribPointer registered VBO as
    // the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); // This is optional, but it's a good practice to unbind any buffers to prevent bugs

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens
    glBindVertexArray(0);

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

        myShader.setFloat("xOffset", offset);

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

unsigned int compileShader(unsigned int type, const char *source)
{
    // Create shader
    unsigned int shader = glCreateShader(type);
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
unsigned int createShaderProgram(const char *vertexShaderSource, const char *fragmentShaderSource)
{
    // Compile shaders
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // Create shader program
    unsigned int program = glCreateProgram();
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