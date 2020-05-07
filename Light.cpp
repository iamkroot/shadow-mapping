#include "Light.h"

Light::Light(const glm::vec3 &position, const glm::vec3 &color, const glm::vec3 &target) :
        position(position),
        color(color),
        target(target),
        model(glm::translate(glm::mat4(1), position)),
        proj_view(glm::mat4(1)),
        isEnabled(true) {}

glm::mat4 Light::lookAt() const {
    return glm::lookAt(position, target, {0, 1, 0});
}

void Light::genDepthMap(unsigned int resolution) {
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

const glm::vec3 &Light::getPosition() const {
    return position;
}

const glm::vec3 &Light::getColor() const {
    return color;
}

GLuint Light::getDepthMap() const {
    return depthMap;
}

const glm::mat4 &Light::getModel() const {
    return model;
}
