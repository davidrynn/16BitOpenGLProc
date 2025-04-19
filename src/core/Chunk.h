#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Shader.h"

class Chunk {
public:

    Chunk(int x, int z);  // world chunk coordinates
    ~Chunk();

    void generate();
    void render(Shader& shader);
    glm::vec2 getChunkPos() const;

private:
    int chunkX, chunkZ;  // world chunk coords
    float spacing; // Adjust this value to control the density of the terrain
    GLuint VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<unsigned int> indices;

    void setupBuffers();
    void drawChunkBoundingBox(); // Add drawChunkBoundingBox method
};

#endif