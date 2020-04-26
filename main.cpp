#include "Camera.h"
#include "GLUtils.h"
#include "ObjModel.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

struct Light {
    glm::vec3 position;
    glm::vec3 color;
};

int main() {
    unsigned int windowWidth = 800, windowHeight = 600;
    float aspect = (float) windowWidth / (float) windowHeight;
    auto window = initGLWindow(windowWidth, windowHeight, "shadows");
    glEnable(GL_DEPTH_TEST);  // enable depth-testing
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    Light light = {{10, 15, 15},
                   {1,  1,  1}};
    Camera camera({10, 10, 5}, {0, 0, 0});
    Shader shader("shaders/standard_vert.glsl", "shaders/standard_frag.glsl");
    Shader depthShader("shaders/depth_vert.glsl", "shaders/depth_frag.glsl");
    auto sphere = ObjModel("models/sphere.obj");
    auto room = ObjModel("models/room.obj");
    auto cube = ObjModel("models/cube.obj");

    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    const unsigned int SHADOW_RES = 1024;

    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_RES, SHADOW_RES, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);  // don't need color buf for shadows
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shader.set("shadowMap", 0);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        auto model = glm::mat4(1.0f);
        float near_plane = 1.f, far_plane = 100.f;
        // PASS 1: Light view - generate depth map
        auto lightProj = glm::perspective(30.f, aspect, near_plane, far_plane);
        auto lightView = glm::lookAt(light.position, {0, 0, 0}, {0, 1, 0});
        auto lightSpace = lightProj * lightView;
        depthShader.use();
        depthShader.set("model", model);
        depthShader.set("lightSpace", lightSpace);

        glViewport(0, 0, SHADOW_RES, SHADOW_RES);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);  // only drawing depth map
        room.draw();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // PASS 2: Full render with shadow
        glViewport(0, 0, windowWidth, windowHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.set("lightPos", light.position);
        shader.set("lightColor", light.color);
        shader.set("lightSpace", lightSpace);
        auto proj = glm::perspective(45.f * camera.getZoom(), aspect, near_plane, far_plane);
        auto view = camera.lookAt();
        shader.set("projection", proj);
        shader.set("view", view);
        shader.set("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        room.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
