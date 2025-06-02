#ifndef PLAYER_H
#define PLAYER_H

#include "Camera.h"
#include <memory>

class Player {
public:
    explicit Player(Camera* camera);
    ~Player() = default;

    // Movement methods
    void moveForward(float delta);
    void moveBackward(float delta);
    void moveLeft(float delta);
    void moveRight(float delta);
    void moveUp(float delta);
    void moveDown(float delta);

    // State methods
    void update(float deltaTime, float terrainHeightAtPlayerXZ);
    void jump();
    void toggleFlyMode();
    void applyForce(const glm::vec3& force);
    
    // Getters
    bool isGrounded() const { return grounded; }
    bool isInFlyMode() const { return flyMode; }
    const glm::vec3& getPosition() const { return camera->getPosition(); }
    
    // Camera access (for rendering and input systems)
    Camera* getCamera() const { return camera; }

private:
    Camera* camera;
    glm::vec3 velocity{0.0f};
    bool flyMode = false;
    bool grounded = false;
    
    // Physics constants
    static constexpr float GRAVITY = -20.0f;
    static constexpr float JUMP_VELOCITY = 10.0f;
    static constexpr float PLAYER_HEIGHT = 2.0f;
    static constexpr float MAX_SPEED = 50.0f;
};

#endif
