#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>

class Terrain;
class FastNoiseLite;

class GrassSpawner {
public:
    GrassSpawner(std::shared_ptr<Terrain> terrain, int density = 2);

    void generate();
    const std::vector<glm::vec3>& getGrassPositions() const;

private:
    std::shared_ptr<Terrain> terrain;
    std::vector<glm::vec3> grassPositions;
    int density; // e.g., one blade every N tiles
};
