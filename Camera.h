#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

class Camera {
    GLfloat yaw;
    GLfloat pitch;
    GLfloat initYaw;
    GLfloat initPitch;
    GLfloat translationSensitivity = 1;
    GLfloat rotationSensitivity = 0.2;
    GLfloat zoomSensitivity = 0.001;
    GLfloat zoomFactor = 1;
    glm::vec3 position;
    glm::vec3 initPosition;
    glm::vec3 up;
    glm::vec3 worldUp{0, 1, 0};
    glm::vec3 front;
    glm::vec3 right;

    void updateVectors();

public:
    /**
     * @brief Simple enum to define a direction of movement
     */
    enum class Direction {
        FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN
    };

    /**
     * @brief Camera class
     * @param position Initial location of the camera
     * @param target Co-ordinates of where the camera is pointing
     * @param worldUp Up vector for world view
     */
    Camera(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &worldUp = {0, 1, 0});

    /**
     * @brief Move the camera along either up, front or right axis.
     * @param direction
     */
    void translate(Camera::Direction direction);

    /**
     * @brief Calculate the final MVP matrix and call gluLookAt. Call this in draw loop.
     */
    glm::mat4 lookAt();

    /**
     * @brief Reset the camera view back to initial parameters.
     */
    void reset();

    /**
     * @brief Modify the yaw and pitch of the camera.
     * @param x Horizontal offset of the movement
     * @param y Vertical offset of the movement
     */
    void rotate(int x, int y);

    /**
     * @brief Change the perspective angle.
     * @param y Vertical offset of the movement
     */
    void zoom(int y);

    /**
     * @brief Get the zoom factor of the camera, to be used for gluPerspective
     */
    [[nodiscard]] GLfloat getZoom() const;
};

#endif //CAMERA_H
