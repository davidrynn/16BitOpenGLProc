#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include "Shader.h"

class EarthGlob {
public:
    void initialize(const glm::vec3& startPos);
    void update(float deltaTime);
    void render(const glm::mat4& view, const glm::mat4& projection);
    
    // Getters for position and state
    glm::vec3 getPosition() const { return position; }
    bool isActive() const { return active; }

private:
    glm::vec3 position;
    glm::vec3 velocity{0.0f};
    float liftSpeed = 1.5f;
    float size = 0.5f;  // Make the glob a bit smaller
    bool active = true;
    
    // OpenGL resources
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;  // Added EBO member
    std::unique_ptr<Shader> shader;
};
