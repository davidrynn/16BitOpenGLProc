#include "MountainsNoise.h"
#include <cmath> // For sin/cos
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp> 

float MountainsNoise::getHeight(float x, float z) const {
    float base = glm::perlin(glm::vec2(x, z) * 0.05f); // Base noise
    float detail = glm::perlin(glm::vec2(x, z) * 0.2f); // Higher frequency
    return base * 10.0f + detail * 3.0f; // Create large mountains with bumps
}
