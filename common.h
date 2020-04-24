#ifndef COMMON_H
#define COMMON_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#ifndef NDEBUG  // set by CMake
#define casePrint(val) case val: std::cout<<#val<<std::endl; break;
#endif

#endif //COMMON_H
