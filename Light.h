#ifndef LIGHT_H
#define LIGHT_H

#include "common.h"
#include "Shader.h"

class Light {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 target;
    GLuint depthMap{0};
    glm::mat4 model;

public:
    glm::mat4 proj_view;

    Light(const glm::vec3 &position, const glm::vec3 &color, const glm::vec3 &target = {0, 0, 0});

    void genDepthMap(unsigned int resolution);

    [[nodiscard]] const glm::vec3 &getPosition() const;

    [[nodiscard]] const glm::vec3 &getColor() const;

    [[nodiscard]] GLuint getDepthMap() const;

    [[nodiscard]] const glm::mat4 &getModel() const;

    [[nodiscard]] glm::mat4 lookAt() const;
};

#endif //LIGHT_H
