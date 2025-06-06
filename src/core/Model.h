#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "core/Shader.h"

// Forward declarations
namespace tinygltf {
    class Model;
    class Node;
    struct Mesh;
}

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec4 weights{0.0f};     // Bone weights (max 4 influences)
    glm::ivec4 joints{-1};       // Bone indices (max 4 influences)
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t textureId;

    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    void Draw(Shader& shader);

private:
    uint32_t VAO, VBO, EBO;
    void setupMesh();
};

class Model {
public:
    Model(const char* path);
    ~Model();
    void Draw(Shader& shader);

private:
    std::vector<std::unique_ptr<Mesh>> meshes;
    std::unique_ptr<tinygltf::Model> gltf;
    
    void loadModel(const std::string& path);
    void processNode(const tinygltf::Node& node, const tinygltf::Model& model);
    std::unique_ptr<Mesh> processMesh(const tinygltf::Mesh& mesh, const tinygltf::Model& model);
    uint32_t loadTexture(const tinygltf::Model& model, int textureIndex);
}; 