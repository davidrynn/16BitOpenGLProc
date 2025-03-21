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
    void update();


private:
    Camera* camera;
};

#endif
