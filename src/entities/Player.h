#ifndef PLAYER_H
#define PLAYER_H

#include "Camera.h"

class Player {
public:
    Player(Camera* camera);

    void moveForward(float delta);
    void moveBackward(float delta);
    void moveLeft(float delta);
    void moveRight(float delta);
    void moveUp(float delta);
    void moveDown(float delta);

    void update(float deltaTime, float terrainHeightAtPlayerXZ);
    void jump();
    void toggleFlyMode();
    
    Camera* camera;

    bool isGrounded = false;
    bool isInFlyMode() const { return flyMode; }

private:
    float velocityY = 0.0f;
    bool flyMode = false;
    const float gravity = -20.0f;      // Tuned for game feel
    const float jumpVelocity = 10.0f;
    const float playerHeight = 2.0f;   // Height from terrain
};

#endif
