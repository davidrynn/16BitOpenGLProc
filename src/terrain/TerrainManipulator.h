#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "Terrain.h"
#include "EarthGlob.h"

class TerrainManipulator {
public:
    void initialize(std::shared_ptr<Terrain> terrain);
    void beginLift(const glm::vec3& worldPosition); // Called after 3s hold
    void update(float deltaTime);
    void render(const glm::mat4& view, const glm::mat4& projection);

private:
    std::shared_ptr<Terrain> terrain;
    std::vector<std::unique_ptr<EarthGlob>> activeGlobs;
};
