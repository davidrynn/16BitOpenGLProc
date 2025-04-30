#include "DesertNoise.h"

DesertNoise::DesertNoise(std::shared_ptr<BaseNoise> base)
    : baseNoise(std::move(base))
{
}

float DesertNoise::getNoise(float x, float z) const
{
    float base = baseNoise->getNoise(x * 0.03f, z * 0.03f); // lower freq
    return base * 2.0f; // flatter terrain
}