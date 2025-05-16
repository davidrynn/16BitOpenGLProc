#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>

class Shader;

class GrassRenderer {
public:
    GrassRenderer();
    ~GrassRenderer();

    void initialize();
    void update(const std::vector<glm::vec3>& newPositions);
    void render(const glm::mat4& view, const glm::mat4& projection, float time);

private:
    unsigned int VAO, VBO, instanceVBO;
    std::unique_ptr<Shader> shader;
    std::vector<glm::vec3> grassPositions;

    void setupBlade();
    void updateInstanceBuffer();
};
