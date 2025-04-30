#include "Player.h"
#include <GL/glew.h> 

Player::Player(Camera* camera)
    : camera(camera)
{
}

void Player::moveForward(float delta)
{
    camera->position += delta * camera->front;
}

void Player::moveBackward(float delta)
{
    camera->position -= delta * camera->front;
}

void Player::moveLeft(float delta)
{
    camera->position -= glm::normalize(glm::cross(camera->front, camera->up)) * delta;
}

void Player::moveRight(float delta)
{
    camera->position += glm::normalize(glm::cross(camera->front, camera->up)) * delta;
}

void Player::moveUp(float delta)
{
    camera->position += camera->up * delta;
}

void Player::moveDown(float delta)
{
    camera->position -= camera->up * delta;
}

void Player::update()
{


}