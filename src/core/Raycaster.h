#pragma once

#include <glm/glm.hpp>
#include <optional>
#include "Camera.h"
#include "Terrain.h"

class Raycaster
{
public:
    // Performs a raycast from the center of the screen and returns a world-space hit point
    static std::optional<glm::vec3> raycastToTerrain(
        const Camera &camera,
        std::shared_ptr<Terrain> terrain,
        float maxDistance = 100.0f,
        float step = 0.5f);
};
