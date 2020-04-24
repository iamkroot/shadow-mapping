#include "GLUtils.h"

int main() {
    auto window = initGLWindow(800, 600, "shadows");
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
