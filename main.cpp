#include "Camera.h"
#include "GLUtils.h"
#include "ObjModel.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

int main() {
    unsigned int windowWidth = 800, windowHeight = 600;
    auto window = initGLWindow(windowWidth, windowHeight, "shadows");
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    Camera camera({20, 20, 20}, {0, 0, 0});
    auto shader = Shader("shaders/standard_vert.glsl", "shaders/standard_frag.glsl");
    auto sphere = ObjModel("models/sphere.obj");
    auto room = ObjModel("models/room.obj");
    auto cube = ObjModel("models/cube.obj");

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();

        auto proj = glm::perspective(45.f * camera.getZoom(), (float) windowWidth / (float) windowHeight, 0.1f, 100.f);
        auto view = camera.lookAt();
        shader.set("projection", proj);
        shader.set("view", view);
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, {5, 5, 5});
        shader.set("model", model);
        room.draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
