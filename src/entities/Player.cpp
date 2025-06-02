#include "Player.h"
#include <glad/glad.h>
#include <iostream>

Player::Player(Camera* camera)
    : camera(camera)
{
}

void Player::moveForward(float delta)
{
    glm::vec3 pos = camera->getPosition();
    pos += delta * camera->getFront();
    camera->setPosition(pos);
}

void Player::moveBackward(float delta)
{
    glm::vec3 pos = camera->getPosition();
    pos -= delta * camera->getFront();
    camera->setPosition(pos);
}

void Player::moveLeft(float delta)
{
    glm::vec3 pos = camera->getPosition();
    pos -= glm::normalize(glm::cross(camera->getFront(), camera->getUp())) * delta;
    camera->setPosition(pos);
}

void Player::moveRight(float delta)
{
    glm::vec3 pos = camera->getPosition();
    pos += glm::normalize(glm::cross(camera->getFront(), camera->getUp())) * delta;
    camera->setPosition(pos);
}

void Player::moveUp(float delta)
{
    glm::vec3 pos = camera->getPosition();
    pos += camera->getUp() * delta;
    camera->setPosition(pos);
}

void Player::moveDown(float delta)
{
    glm::vec3 pos = camera->getPosition();
    pos -= camera->getUp() * delta;
    camera->setPosition(pos);
}

void Player::update(float deltaTime, float terrainHeightAtPlayerXZ)
{
    if (!flyMode)
    {
        // Apply gravity
        velocity.y += GRAVITY * deltaTime;

        // Move player
        glm::vec3 pos = camera->getPosition();
        pos += velocity * deltaTime;
        camera->setPosition(pos);

        // Terrain collision
        float groundLevel = terrainHeightAtPlayerXZ + PLAYER_HEIGHT;
        pos = camera->getPosition();

        if (pos.y < groundLevel)
        {
            pos.y = groundLevel;
            camera->setPosition(pos);
            velocity.y = 0.0f;
            grounded = true;
        }
        else
        {
            grounded = false;
        }
    }
    else
    {
        grounded = true; // Allow jumping in air during fly mode
    }

    // Apply drag and speed limit
    velocity *= 0.99f;
    if (glm::length(velocity) > MAX_SPEED)
    {
        velocity = glm::normalize(velocity) * MAX_SPEED;
    }
}

void Player::jump()
{
    if (grounded || flyMode)
    {
        velocity.y = JUMP_VELOCITY;
        grounded = false;
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

void Player::applyForce(const glm::vec3& force)
{
    // Clamp velocity to max speed
    glm::vec3 newVelocity = velocity + force;
    float newSpeed = glm::length(newVelocity);
    
    if (newSpeed > MAX_SPEED)
    {
        velocity = glm::normalize(newVelocity) * MAX_SPEED;
    }
    else
    {
        velocity = newVelocity;
    }
}