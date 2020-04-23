cmake_minimum_required(VERSION 3.16)
project(shadows)

set(CMAKE_CXX_STANDARD 20)

add_executable(shadows main.cpp)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
include_directories(shadows GLM_INCLUDE_DIR)

target_link_libraries(shadows OpenGL::OpenGL OpenGL::GLU GLUT::GLUT GLEW::GLEW)
