#ifndef LIGHT_H
#define LIGHT_H

#include "common.h"
#include "Shader.h"

/**
 * @brief Class to describe the light source
 */
class Light
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 target;
    GLuint depthMap{0};
    glm::mat4 model;

public:
    bool isEnabled;
    glm::mat4 proj_view;

    /**
     * @brief Consturctor for the class
     * @param position The position of the light source
     * @param color RGB color of the light source
     * @param target The point light source are trying to be focused on
     */
    Light(const glm::vec3 &position, const glm::vec3 &color, const glm::vec3 &target = {0, 0, 0});

    /**
     * @brief Method to generate the depth map
     * @param resolution The resolution of the map
     */
    void genDepthMap(unsigned int resolution);

    /**
     * @brief Method to get the position of the light source
     */
    [[nodiscard]] const glm::vec3 &getPosition() const;

    /**
     * @brief Method to returm the color of the light
     */
    [[nodiscard]] const glm::vec3 &getColor() const;

    /**
     * @brief Method to get the depth map
     */
    [[nodiscard]] GLuint getDepthMap() const;

    /**
     * @brief Method to get the model matrix
     */
    [[nodiscard]] const glm::mat4 &getModel() const;

    /**
     * @brief Method to get the view matrix from light source's point of view
     */
    [[nodiscard]] glm::mat4 lookAt() const;
};

#endif //LIGHT_H
