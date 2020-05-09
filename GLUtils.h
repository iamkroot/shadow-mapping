#ifndef GLUTILS_H
#define GLUTILS_H

#include "common.h"

/**
 * @brief Callback to log all the messages by opengl for easy debugging
 */
void GLAPIENTRY
glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
              const void* userParam);

/**
 * @brief Method to initialize a glfw Window
 * @param width Width of the window
 * @param height Height of the window
 * @param title Title of the window
 */
GLFWwindow* initGLWindow(GLuint width, GLuint height, const char* title);

#endif //GLUTILS_H
