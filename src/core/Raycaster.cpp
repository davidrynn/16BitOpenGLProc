#include "Raycaster.h"
#include <glm/glm.hpp>

std::optional<glm::vec3> Raycaster::raycastToTerrain(
    const Camera& camera,
    std::shared_ptr<Terrain> terrain,
    float maxDistance,
    float step)
{
    if (!terrain) return std::nullopt;

    glm::vec3 rayOrigin = camera.getPosition();
    glm::vec3 rayDirection = camera.getFront();
    
    for (float distance = 0.0f; distance < maxDistance; distance += step)
    {
        glm::vec3 currentPoint = rayOrigin + rayDirection * distance;
        float terrainHeight = terrain->getHeightAt(currentPoint.x, currentPoint.z);
        
        if (currentPoint.y <= terrainHeight)
        {
            return currentPoint;
        }
    }
    
    return std::nullopt;
}
