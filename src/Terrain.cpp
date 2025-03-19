#include "Terrain.h"
#include <vector>

std::vector<float> Terrain::generateTerrain(int gridSize) {
    std::vector<float> vertices;
    float scale = 2.0f / (gridSize - 1);  // Normalize to range -1 to 1

    for (int z = 0; z < gridSize; z++) {
        for (int x = 0; x < gridSize; x++) {
            float xpos = x * scale - 1.0f;
            float zpos = z * scale - 1.0f;
            float ypos = sin(xpos * 3.0f) * cos(zpos * 3.0f) * 0.2f; // Wavy effect

            vertices.push_back(xpos);
            vertices.push_back(ypos);
            vertices.push_back(zpos);
        }
    }
    return vertices;
}

// ✅ Add the missing function definition
std::vector<unsigned int> Terrain::generateIndices(int gridSize) {
    std::vector<unsigned int> indices;

    for (int z = 0; z < gridSize - 1; z++) {
        for (int x = 0; x < gridSize - 1; x++) {
            int topLeft = z * gridSize + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * gridSize + x;
            int bottomRight = bottomLeft + 1;

            // Two triangles per grid square
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
    return indices;
}
