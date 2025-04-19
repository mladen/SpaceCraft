#include <glad/glad.h>  // For OpenGL functions
#include <GLFW/glfw3.h> // For GLFW functions (e.g., GLFWwindow, glfwCreateWindow) which help with window creation
#include <cmath>        // For math functions
#include <iostream>     // For console output
#include <vector>       // For std::vector, a dynamic array (for storing vertices, colors, etc.) which help with dynamic memory allocation

// Window dimensions
const int WIDTH = 1368;
const int HEIGHT = 768;

// Circle properties
const float RADIUS = 0.5f;
const int NUM_SEGMENTS = 100;
const float PI = 3.14159265359f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height); // Callback function for window resize
void processInput(GLFWwindow *window);                                     // Callback function for keyboard input

// Vertex shader source
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\0";

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
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Spacecraft", NULL, NULL);
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

    // Set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // Generate and configure buffers
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO); // VAO = Vertex Array Object; stores the state of the vertex attribute configuration and uses it when rendering
    glGenBuffers(1, &VBO);      // VBO = Vertex Buffer Object; stores the vertex data
    glGenBuffers(1, &EBO);      // EBO = Element Buffer Object; stores the index data

    // Bind VAO first, then bind and set VBO(s) and EBO, then configure vertex attributes
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute which is used in the vertex shader to position the vertices
    // The last parameter (GL_FALSE) tells OpenGL that the data is not normalized, meaning that it should stay between 0 and 1
    // The stride parameter (3 * sizeof(float)) tells OpenGL how many bytes to skip in the buffer to get to the next vertex
    // The offset parameter (0) tells OpenGL where to start in the buffer
    // The first parameter (0) is the index of the vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0); // Enable the vertex attribute

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Process input
        processInput(window);

        // Rendering commands
        // glClearColor(0.072f, 0.13f, 0.17f, 1.0f);
        glClearColor(0.0f, 0.875f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use shader program
        glUseProgram(shaderProgram); // Every shader and rendering call after glUseProgram will
        // now use this program object (and thus the shaders).

        // Set the polygon mode which draws a wireframe
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // GL_LINE, GL_FILL

        // Draw rectangle
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll events
        glfwSwapBuffers(window); // Swaps the front and back buffers
        glfwPollEvents();        // Polls for and processes events
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
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