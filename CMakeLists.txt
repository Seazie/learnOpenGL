cmake_minimum_required(VERSION 3.14)
project(openGLTest)

set(CMAKE_CXX_STANDARD 17)

add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
add_executable(openGLTest main.cpp)

target_link_libraries(openGLTest PRIVATE glad glfw OpenGL::GL)