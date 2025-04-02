#include "PlainsNoise.h"
#include <cmath>
#include <glm/gtc/noise.hpp>

float PlainsNoise::getHeight(float x, float z) const {
    float base = glm::perlin(glm::vec2(x, z) * 0.03f); // Low-frequency noise
    return base * 2.0f; // Keep it very subtle for flat terrain
}