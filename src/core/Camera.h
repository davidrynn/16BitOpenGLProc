#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(const glm::vec3& position = glm::vec3(0.0f));
    
    // Position methods
    const glm::vec3& getPosition() const { return position; }
    void setPosition(const glm::vec3& newPosition) { position = newPosition; }
    
    // Direction methods
    const glm::vec3& getFront() const { return front; }
    const glm::vec3& getUp() const { return up; }
    const glm::vec3& getRight() const { return right; }
    
    // Camera control
    void updateDirection(float yaw, float pitch);
    glm::mat4 getViewMatrix() const;

private:
    void updateCameraVectors();

    glm::vec3 position;
    glm::vec3 front{0.0f, 0.0f, -1.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    glm::vec3 right{1.0f, 0.0f, 0.0f};
    glm::vec3 worldUp{0.0f, 1.0f, 0.0f};
    
    float yaw = -90.0f;
    float pitch = 0.0f;
};

#endif
