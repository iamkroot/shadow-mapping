#include "Camera.h"
#include "GLUtils.h"
#include "ObjModel.h"
#include "Shader.h"
#include "Light.h"

Camera camera({10, 10, -5}, {0, 0, 0});
std::vector<Light> lights = {
        {{10,  15, 15},  {0, 0, 1}},
        {{-10, 15, -15}, {1, 0, 0}},
};

bool drawMinimaps = true;

/** 
 * @brief Custom callback function for a window
 * @param window The window to bind the callback to
 * @param key The key responsible for triggering the callback
 * @param action The action related to the key triggering the callback
 */
void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

/**
 * @brief Custom callback for a window for mouse
 * @param window Window to which the callback is bound to
 * @param button Mouse button associated
 * @param action The action associated with the mouse button
 */
void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

/**
 * @brief Callback for changing cursor position
 * @param window Window to which the callback is bound to
 * @param xPos x coordinate of the cursor
 * @param yPos y coordinate of the cursor
 */
void glfw_cursor_position_callback(GLFWwindow* window, double xPos, double yPos);

/**
 * @brief Callback for scrolling
 * @param window Window to which the callback is bound to
 * @param xOffset x axis offset
 * @param yOffset y axis offset
 */
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

    Shader shader("shaders/standard_vert.glsl", "shaders/standard_frag.glsl");
    Shader depthShader("shaders/depth_vert.glsl", "shaders/depth_frag.glsl");
    Shader minimapShader("shaders/standard_vert.glsl", "shaders/minimap_depth_frag.glsl");
    Shader lightShader("shaders/standard_vert.glsl", "shaders/light_frag.glsl");
    auto room = ObjModel("models/room.obj");
    auto cube = ObjModel("models/cube.obj");

    unsigned int depthMapFBO = 0;
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glDrawBuffer(GL_NONE); // don't need color buf for shadows
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    const unsigned int SHADOW_RES = 1024;

    for (auto &light : lights) {
        light.genDepthMap(SHADOW_RES);
    }

    shader.set("numLights", (int) lights.size());

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        auto model = glm::mat4(1.0f);
        float near_plane = 1.f, far_plane = 35.f;

        // PASS 1: Light view - generate depth maps for each light
        for (auto &light : lights) {
            if (not light.isEnabled)
                continue;
            auto lightProj = glm::perspective(30.f, aspect, near_plane, far_plane);
            auto lightView = light.lookAt();
            light.proj_view = lightProj * lightView;
            depthShader.use();
            depthShader.set("model", model);
            depthShader.set("lightSpace", light.proj_view);

            // attach depth map texture to framebuffer and render the scene from light's PoV
            glViewport(0, 0, SHADOW_RES, SHADOW_RES);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, light.getDepthMap(), 0);
            glClear(GL_DEPTH_BUFFER_BIT); // only drawing depth map
            room.draw();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        // PASS 2: Main render with shadows
        glViewport(0, 0, windowWidth, windowHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto proj = glm::perspective(45.f * camera.getZoom(), aspect, near_plane, far_plane);
        auto view = camera.lookAt();
        shader.set("projection", proj);
        shader.set("view", view);
        shader.set("model", model);
        lightShader.set("projection", proj);
        lightShader.set("view", view);
        lightShader.use();
        for (int i = 0; i < lights.size(); ++i) {
            auto light = lights[i];
            const std::string prefix = "lights[" + std::to_string(i) + "].";
            shader.set(prefix + "enabled", light.isEnabled);
            if (not light.isEnabled) {
                continue;
            }
            shader.set(prefix + "lightPos", light.getPosition());
            shader.set(prefix + "lightColor", light.getColor());
            shader.set(prefix + "lightSpace", light.proj_view);
            shader.set(prefix + "shadowMap", i);  // should be the index used in glActiveTexture, not the texture ID

            // draw light using a cube
            lightShader.set("model", light.getModel());
            lightShader.set("lightColor", light.getColor());
            cube.draw();

            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, light.getDepthMap());
        }
        shader.use();
        room.draw();

        // Draw minimaps with z-values for each light
        if (drawMinimaps) {
            minimapShader.set("projection", proj);
            minimapShader.set("view", view);
            minimapShader.set("model", model);
            minimapShader.set("near_plane", near_plane);
            minimapShader.set("far_plane", far_plane);
            minimapShader.use();
            auto minimapWidth = windowWidth / 4, minimapHeight = windowHeight / 4;
            glEnable(GL_SCISSOR_TEST);
            for (int i = 0; i < lights.size(); ++i) {
                if (not lights[i].isEnabled)
                    continue;
                // clear small rectangle on the window
                glViewport(i * minimapWidth, 0, minimapWidth, minimapHeight);
                glScissor(i * minimapWidth, 0, minimapWidth, minimapHeight);
                glClearColor(0.f, 0.f, 0.f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                minimapShader.set("lightSpace", lights[i].proj_view);
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
        drawMinimaps = not drawMinimaps;
    } else { // toggle lights using number keys
        for (int i = 0; i < lights.size(); ++i) {
            if (key == GLFW_KEY_1 + i and action == GLFW_PRESS) {
                lights[i].isEnabled = not lights[i].isEnabled;
                break;
            }
        }
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
