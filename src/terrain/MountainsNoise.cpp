#include "MountainsNoise.h"

MountainsNoise::MountainsNoise(std::shared_ptr<BaseNoise> base)
    : baseNoise(std::move(base))
{
}

float MountainsNoise::getNoise(float x, float z) const
{
    float base = baseNoise->getNoise(x * 0.05f, z * 0.05f);
    float detail = baseNoise->getNoise(x * 0.2f, z * 0.2f);
    return base * 10.0f + detail * 3.0f; // taller mountains with bumps
}
