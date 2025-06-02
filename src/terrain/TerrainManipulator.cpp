#include "TerrainManipulator.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

void TerrainManipulator::initialize(std::shared_ptr<Terrain> terrainPtr) {
    terrain = terrainPtr;
}

void TerrainManipulator::beginLift(const glm::vec3& worldPosition) {
    std::cout << "[TerrainManipulator] Lifting terrain at: " << glm::to_string(worldPosition) << std::endl;

    auto glob = std::make_unique<EarthGlob>();
    glob->initialize(worldPosition);
    activeGlobs.push_back(std::move(glob));

    // TODO: Deform terrain by creating a crater/hole
    // This would involve modifying the terrain height data
    // and regenerating the affected chunks
}

void TerrainManipulator::update(float deltaTime) {
    for (auto it = activeGlobs.begin(); it != activeGlobs.end();) {
        (*it)->update(deltaTime);
        
        glm::vec3 pos = (*it)->getPosition();
        if (pos.y > 100.0f) { // Example height limit
            it = activeGlobs.erase(it);
        } else {
            ++it;
        }
    }
}

void TerrainManipulator::render(const glm::mat4& view, const glm::mat4& projection) {
    for (const auto& glob : activeGlobs) {
        glob->render(view, projection);
    }
}
