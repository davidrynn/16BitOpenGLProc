#ifndef CHUNK_H
#define CHUNK_H

#include <memory>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Shader.h"

class Terrain;  // Forward declaration

class Chunk {
public:
    Chunk(int x, int z, std::shared_ptr<Terrain> terrain);  // Accept Terrain as reference
    ~Chunk();

    void render(Shader& shader);
    glm::vec2 getChunkPos() const;

private:
    int chunkX, chunkZ;
    float spacing;
    GLuint VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<unsigned int> indices;

    std::shared_ptr<Terrain> terrain;  // Store reference

    void generate();         // No longer needs parameter
    void setupBuffers();
    void drawChunkBoundingBox();
};

#endif
