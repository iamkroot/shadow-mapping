#include "GLUtils.h"
#include <iostream>

void GLAPIENTRY
glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
              const void* userParam) {
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;
    std::cout << "Source: ";
    switch (source) {
        casePrint(GL_DEBUG_SOURCE_API)
        casePrint(GL_DEBUG_SOURCE_WINDOW_SYSTEM)
        casePrint(GL_DEBUG_SOURCE_SHADER_COMPILER)
        casePrint(GL_DEBUG_SOURCE_THIRD_PARTY)
        casePrint(GL_DEBUG_SOURCE_APPLICATION)
        casePrint(GL_DEBUG_SOURCE_OTHER)
    }
    std::cout << "Type: ";
    switch (type) {
        casePrint(GL_DEBUG_TYPE_ERROR)
        casePrint(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR)
        casePrint(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR)
        casePrint(GL_DEBUG_TYPE_PORTABILITY)
        casePrint(GL_DEBUG_TYPE_PERFORMANCE)
        casePrint(GL_DEBUG_TYPE_MARKER)
        casePrint(GL_DEBUG_TYPE_PUSH_GROUP)
        casePrint(GL_DEBUG_TYPE_POP_GROUP)
        casePrint(GL_DEBUG_TYPE_OTHER)
    }
    std::cout << "Severity: ";
    switch (severity) {
        casePrint(GL_DEBUG_SEVERITY_HIGH)
        casePrint(GL_DEBUG_SEVERITY_MEDIUM)
        casePrint(GL_DEBUG_SEVERITY_LOW)
        casePrint(GL_DEBUG_SEVERITY_NOTIFICATION)
    }
}

GLFWwindow* initGLWindow(GLuint width, GLuint height, const char* title) {
    if (!glfwInit()) {
        throw std::runtime_error("Error while initializing GLFW");
    }
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Error while creating window with GLFW");
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        glfwTerminate();
        throw std::runtime_error((const char*) glewGetErrorString(err));
    }
#ifndef NDEBUG
    // enable GL debug logging
    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif
    return window;
}
