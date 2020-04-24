#ifndef GLUTILS_H
#define GLUTILS_H

#include "common.h"

/**
 * @brief Callback to log all the messages by opengl for easy debugging
 */
void GLAPIENTRY
glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
              const void* userParam);

GLFWwindow* initGLWindow(GLuint width, GLuint height, const char* title);

#endif //GLUTILS_H
