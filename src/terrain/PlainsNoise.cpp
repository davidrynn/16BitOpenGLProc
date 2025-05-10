#include "PlainsNoise.h"
#include <iostream>

PlainsNoise::PlainsNoise(std::shared_ptr<BaseNoise> base)
    : baseNoise(std::move(base))
{
}

float PlainsNoise::getNoise(float x, float z) const
{
    float base = baseNoise->getNoise(x * 0.2f, z * 0.2f); // higher frequency
    float result = base * 5.0f; // lower amplitude
    return result;
}
