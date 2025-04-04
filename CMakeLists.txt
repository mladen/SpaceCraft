cmake_minimum_required(VERSION 3.16)

# Specify GCC-11 as the preferred compiler before project definition
set(CMAKE_CXX_COMPILER "/usr/bin/g++-12")

project(C-raft CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Use standard C++20 instead of compiler-specific extensions

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp" "include/*.h")

# Include directories
include_directories(include)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
find_package(glfw3 3.3 REQUIRED)

# Find other dependencies
find_package(GLEW REQUIRED)  # For OpenGL extension loading
find_package(glm REQUIRED)   # For mathematics

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    glfw
    GLEW::GLEW
    ${GLM_LIBRARIES}
)

# Enable more warnings and treat warnings as errors
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )
endif()

# Optional: Add support for sanitizers in debug mode
option(ENABLE_SANITIZERS "Enable sanitizers in debug builds" OFF)
if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE MATCHES "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE
            -fsanitize=address,undefined
        )
        target_link_options(${PROJECT_NAME} PRIVATE
            -fsanitize=address,undefined
        )
    endif()
endif()

# Print compiler information
message(STATUS "Using C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

# Installation
install(TARGETS ${PROJECT_NAME} DESTINATION bin)