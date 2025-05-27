#include "Raycaster.h"

std::optional<glm::vec3> Raycaster::raycastToTerrain(
    const Camera& camera,
    std::shared_ptr<Terrain> terrain,
    float maxDistance,
    float step
) {
    glm::vec3 origin = camera.position;
    glm::vec3 direction = glm::normalize(camera.front);

    for (float t = 0.0f; t < maxDistance; t += step) {
        glm::vec3 samplePoint = origin + direction * t;
        float terrainHeight = terrain->getHeightAt(samplePoint.x, samplePoint.z);
        if (samplePoint.y <= terrainHeight) {
            return glm::vec3(samplePoint.x, terrainHeight, samplePoint.z);
        }
    }
    return std::nullopt;
}
