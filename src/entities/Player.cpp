#include "Player.h"
#include <GL/glew.h>
#include <iostream>

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

void Player::update(float deltaTime, float terrainHeightAtPlayerXZ)
{
    if (!flyMode)
    {
        // Apply gravity
        velocityY += gravity * deltaTime;

        // Move player vertically
        camera->position.y += velocityY * deltaTime;

        // Terrain collision
        float groundLevel = terrainHeightAtPlayerXZ + playerHeight;

        if (camera->position.y < groundLevel)
        {
            camera->position.y = groundLevel;
            velocityY = 0.0f;
            isGrounded = true;
        }
        else
        {
            isGrounded = false;
        }
    }
    else
    {
        isGrounded = true; // Allow jumping in air during fly mode
    }
}

void Player::jump()
{
    if (isGrounded || flyMode) {
        velocityY = jumpVelocity;
        isGrounded = false;
    }
}

void Player::toggleFlyMode()
{
    flyMode = !flyMode;
    if (flyMode) {
        velocityY = 0.0f;  // stop gravity
    }
    std::cout << "Fly mode: " << (flyMode ? "ON" : "OFF") << std::endl;
}