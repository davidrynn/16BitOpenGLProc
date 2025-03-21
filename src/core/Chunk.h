#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Chunk {
public:
    static const int SIZE = 32;

    Chunk(int x, int z);  // world chunk coordinates
    ~Chunk();

    void generate();
    void render();
    glm::vec2 getChunkPos() const;

private:
    int chunkX, chunkZ;  // world chunk coords
    GLuint VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    void setupBuffers();
};

#endif