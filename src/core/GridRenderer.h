#pragma once
#include <Gl/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Shader;

class GridRenderer {
public:
    GridRenderer(int size, float spacing);
    ~GridRenderer();

    void render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& color);
    void setVisible(bool visible);
    bool isVisible() const;

private:
    GLuint VAO, VBO;
    int vertexCount;
    bool visible;

    Shader* shader;
    void generateGrid(int size, float spacing);
};
