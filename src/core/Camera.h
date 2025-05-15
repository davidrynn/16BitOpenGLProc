#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(glm::vec3 position);
    void updateDirection(float yaw, float pitch);
    glm::mat4 getViewMatrix() const;

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

private:
glm::vec3 worldUp;
};

#endif
