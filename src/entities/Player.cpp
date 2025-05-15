#include "Player.h"
#include <GL/glew.h>
#include <iostream>

Player::Player(Camera *camera)
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
        velocity.y += gravity * deltaTime;

        // Move player
        camera->position += velocity * deltaTime;

        // Terrain collision
        float groundLevel = terrainHeightAtPlayerXZ + playerHeight;

        if (camera->position.y < groundLevel)
        {
            camera->position.y = groundLevel;
            velocity.y = 0.0f;
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

    velocity *= 0.99f;
    float maxSpeed = 50.0f;
    if (glm::length(velocity) > maxSpeed)
    {
        velocity = glm::normalize(velocity) * maxSpeed;
    }
}

void Player::jump()
{
    if (isGrounded || flyMode)
    {
        velocity.y = jumpVelocity;
        isGrounded = false;
    }
}

void Player::toggleFlyMode()
{
    flyMode = !flyMode;
    if (flyMode)
    {
        velocity.y = 0.0f; // stop gravity
    }
    std::cout << "Fly mode: " << (flyMode ? "ON" : "OFF") << std::endl;
}

void Player::applyForce(const glm::vec3 &force)
{

    // Clamp velocity to max speed
    glm::vec3 newVelocity = velocity + force;

    const float maxSpeed = 60.0f;
    float newSpeed = glm::length(newVelocity);
    if (newSpeed > maxSpeed)
    {
        velocity = glm::normalize(newVelocity) * maxSpeed;
    }
    else
    {
        velocity = newVelocity;
    }
}