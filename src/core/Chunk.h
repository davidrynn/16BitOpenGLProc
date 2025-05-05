#pragma once
#include <memory>
#include <vector>
#include <GL/glew.h> // Will want to remove this for testing.
#include <glm/glm.hpp>

class Shader;
class Terrain;

class Chunk {
public:
    Chunk(int x, int z, std::shared_ptr<Terrain> terrain, bool renderingEnabled = true);
    ~Chunk();
    void generate();
    void uploadToGPU();
    void render(Shader& shader) const;
    bool isUploaded() const { return uploaded; }

private:
    void drawChunkBoundingBox() const;

    bool renderingEnabled;
    bool uploaded = false;
    int chunkX, chunkZ;
    float spacing;
    GLuint VAO, VBO, EBO;
    std::shared_ptr<Terrain> terrain;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<unsigned int> indices;
};