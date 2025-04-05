cmake_minimum_required(VERSION 3.10)    # Sets the minimum required version of CMake to 3.10
project(mycraft++ LANGUAGES CXX C)      # Defines the project name as mycraft++ and specifies it uses both C++ and C

set(CMAKE_CXX_STANDARD 20)          # Enforces the use of C++20 standard
set(CMAKE_CXX_STANDARD_REQUIRED ON) # REQUIRED ensures CMake throws an error if C++20 is not supported

# GLFW
find_package(glfw3 REQUIRED)    # Searches for an installed glfw3 package.
                                # If not found, the configuration stops with an error

# GLAD
add_library(glad STATIC external/glad/src/glad.c)   # Creates a static library named glad using glad.c
                                                    # This handles OpenGL function loading
target_include_directories(glad PUBLIC external/glad/include) # Tells CMake to use the external/glad/include directory for GLAD headers

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")  # Recursively finds all .cpp files in src/
file(GLOB_RECURSE HEADERS "include/*.hpp" "include/*.h")    # Recursively finds all .hpp and .h files in include/
                                        # These files are collected into variables SOURCES and HEADERS

# Include directories
include_directories(include) # Adds include/ to the global include path so that headers can be found without full path prefixes

# Create the executable using the source files .cpp, .hpp and .h
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS}) # Creates an executable with the name mycraft++ using the gathered source and header files

target_link_libraries(${PROJECT_NAME}
    glad    # For GLAD which is a static library for OpenGL
    glfw    # For GLFW which is a dynamic library for windowing and input
    dl      # For dlopen and dlsym which are used by GLFW to load OpenGL
    GL      # For OpenGL
)
