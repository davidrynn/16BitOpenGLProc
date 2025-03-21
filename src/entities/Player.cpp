#include "Player.h"
#include "Terrain.h"

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

void Player::update()
{
  // Snap to terrain height
  float terrainHeight = Terrain::getHeightAt(camera->position.x, camera->position.z);
  camera->position.y = terrainHeight + 1.0f;  // 1.0f is eye/camera height offset

}