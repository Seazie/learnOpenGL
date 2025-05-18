cmake_minimum_required(VERSION 3.10)
project(learnOpenGL)

# Add GLFW
add_subdirectory(libs/glfw)

# Add your executable
add_executable(learnOpenGL main.cpp glad/src/glad.c)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/glad/include)
target_include_directories(learnOpenGL PRIVATE external/glfw/include glad)

# Link GLFW
target_link_libraries(learnOpenGL PRIVATE glfw)

# Link OpenGL
if (WIN32)
    target_link_libraries(learnOpenGL PRIVATE opengl32)
endif()