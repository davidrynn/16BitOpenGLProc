#include "GrassSpawner.h"
#include "Terrain.h"
#include "FastNoiseLiteWrapper.h"

GrassSpawner::GrassSpawner(std::shared_ptr<Terrain> terrain, int density)
    : terrain(std::move(terrain)), density(density) {}

void GrassSpawner::generate() {
    grassPositions.clear();

    int worldSize = 512; // or get this from terrain if dynamic
    FastNoiseLite noise;
    noise.SetSeed(1337);
    noise.SetFrequency(0.1f);

    for (int x = 0; x < worldSize; x += density) {
        for (int z = 0; z < worldSize; z += density) {
            float chance = noise.GetNoise((float)x, (float)z);
            if (chance > 0.2f) {
                float y = terrain->getHeightAt((float)x, (float)z);
                grassPositions.emplace_back(x, y, z);
            }
        }
    }
}

const std::vector<glm::vec3>& GrassSpawner::getGrassPositions() const {
    return grassPositions;
}
