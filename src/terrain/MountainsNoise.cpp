#include "MountainsNoise.h"

MountainsNoise::MountainsNoise(std::shared_ptr<BaseNoise> base)
    : baseNoise(std::move(base))
{
}

float MountainsNoise::getNoise(float x, float z) const
{
 float height = 0.0f;

    height += baseNoise->getNoise(x * 0.01f, z * 0.01f) * 400.0f;   // base shape
    height += baseNoise->getNoise(x * 0.05f, z * 0.05f) * 30.0f;   // mid-scale bumps
    height += baseNoise->getNoise(x * 0.1f,  z * 0.1f)  * 10.0f;    // fine detail
    return height;
}
