#include "Camera.h"
#include "GLUtils.h"
#include "ObjModel.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

struct Light {
    glm::vec3 position;
    glm::vec3 color;
    glm::mat4 lightSpace;
};

Camera camera({10, 10, -5}, {0, 0, 0});
bool drawMinimap = true;

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void glfw_cursor_position_callback(GLFWwindow* window, double xPos, double yPos);

void glfw_scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

int main() {
    unsigned int windowWidth = 800, windowHeight = 600;
    float aspect = (float) windowWidth / (float) windowHeight;
    auto* window = initGLWindow(windowWidth, windowHeight, "shadows");

    glfwSetKeyCallback(window, glfw_key_callback);
    glfwSetMouseButtonCallback(window, glfw_mouse_button_callback);
    glfwSetCursorPosCallback(window, glfw_cursor_position_callback);
    glfwSetScrollCallback(window, glfw_scroll_callback);
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    std::vector<Light> lights = {
            {{10,  15, 15},  {0, 0, 1}},
            {{-10, 15, -15}, {1, 0, 0}},
    };

    Shader shader("shaders/standard_vert.glsl", "shaders/standard_frag.glsl");
    Shader depthShader("shaders/depth_vert.glsl", "shaders/depth_frag.glsl");
    Shader minimapShader("shaders/standard_vert.glsl", "shaders/minimap_depth_frag.glsl");
    auto room = ObjModel("models/room.obj");

    unsigned int depthMapFBO = 0;
    glGenFramebuffers(1, &depthMapFBO);

    const unsigned int SHADOW_RES = 1024;

    unsigned int depthMaps[lights.size()];
    glGenTextures(lights.size(), depthMaps);
    for (int i = 0; i < lights.size(); ++i) {
        glBindTexture(GL_TEXTURE_2D, depthMaps[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_RES, SHADOW_RES, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                     NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glDrawBuffer(GL_NONE); // don't need color buf for shadows
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shader.set("numLights", (int) lights.size());

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        auto model = glm::mat4(1.0f);
        float near_plane = 1.f, far_plane = 100.f;

        // PASS 1: Light view - generate depth maps for each light
        for (int i = 0; i < lights.size(); ++i) {
            auto* light = &lights[i];
            auto lightProj = glm::perspective(30.f, aspect, near_plane, far_plane);
            auto lightView = glm::lookAt(light->position, {0, 0, 0}, {0, 1, 0});
            light->lightSpace = lightProj * lightView;
            depthShader.use();
            depthShader.set("model", model);
            depthShader.set("lightSpace", light->lightSpace);

            glViewport(0, 0, SHADOW_RES, SHADOW_RES);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMaps[i], 0);
            glClear(GL_DEPTH_BUFFER_BIT); // only drawing depth map
            room.draw();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        // PASS 2: Full render with shadow
        glViewport(0, 0, windowWidth, windowHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        auto proj = glm::perspective(45.f * camera.getZoom(), aspect, near_plane, far_plane);
        auto view = camera.lookAt();
        shader.set("projection", proj);
        shader.set("view", view);
        shader.set("model", model);
        for (int i = 0; i < lights.size(); ++i) {
            auto* light = &lights[i];
            const std::string prefix = "lights[" + std::to_string(i) + "].";
            shader.set(prefix + "lightPos", light->position);
            shader.set(prefix + "lightColor", light->color);
            shader.set(prefix + "lightSpace", light->lightSpace);
            shader.set(prefix + "shadowMap", i);
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, depthMaps[i]);

        }
        room.draw();

        // Draw minimaps with z-values for each light
        if (drawMinimap) {
            minimapShader.set("projection", proj);
            minimapShader.set("view", view);
            minimapShader.set("model", model);
            minimapShader.set("near_plane", near_plane);
            minimapShader.set("far_plane", far_plane);
            minimapShader.use();
            auto minimapWidth = windowWidth / 4, minimapHeight = windowHeight / 4;
            glEnable(GL_SCISSOR_TEST);
            for (int i = 0; i < lights.size(); ++i) {
                glViewport(i * minimapWidth, 0, minimapWidth, minimapHeight);
                glScissor(i * minimapWidth, 0, minimapWidth, minimapHeight);
                glClearColor(0.f, 0.f, 0.f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                minimapShader.set("lightSpace", lights[i].lightSpace);
                room.draw();
            }
            glDisable(GL_SCISSOR_TEST);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

bool isDragging = false;
double dragX = -1, dragY = -1;

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W and action == GLFW_PRESS) {
        camera.translate(Camera::Direction::FORWARD);
    } else if (key == GLFW_KEY_S and action == GLFW_PRESS) {
        camera.translate(Camera::Direction::BACKWARD);
    } else if (key == GLFW_KEY_A and action == GLFW_PRESS) {
        camera.translate(Camera::Direction::LEFT);
    } else if (key == GLFW_KEY_D and action == GLFW_PRESS) {
        camera.translate(Camera::Direction::RIGHT);
    } else if (key == GLFW_KEY_UP and action == GLFW_PRESS) {
        camera.translate(Camera::Direction::UP);
    } else if (key == GLFW_KEY_DOWN and action == GLFW_PRESS) {
        camera.translate(Camera::Direction::DOWN);
    } else if (key == GLFW_KEY_R and action == GLFW_PRESS) {
        camera.reset();
    } else if (key == GLFW_KEY_Q and action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if (key == GLFW_KEY_M and action == GLFW_PRESS) {
        drawMinimap = not drawMinimap;
    }
}

void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        isDragging = action == GLFW_PRESS;
        if (not isDragging)
            dragX = dragY = -1;
    }
}

void glfw_cursor_position_callback(GLFWwindow* window, double xPos, double yPos) {
    if (isDragging) {
        if (dragX != -1 and dragY != -1)
            camera.rotate(xPos - dragX, yPos - dragY);
        dragX = xPos;
        dragY = yPos;
    }
}

void glfw_scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
    camera.zoom(yOffset);
}
