#include "FastNoiseLiteWrapper.h"

FastNoiseLiteWrapper::FastNoiseLiteWrapper() {
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(0.01f);
    noise.SetSeed(1337);
}

float FastNoiseLiteWrapper::getHeight(float x, float z) const {
    return noise.GetNoise(x, z);
}

float FastNoiseLiteWrapper::getNoise(float x, float z) const {
    return noise.GetNoise(x, z);
}

void FastNoiseLiteWrapper::setSeed(int seed) {
    noise.SetSeed(seed);
}

void FastNoiseLiteWrapper::setFrequency(float frequency) {
    noise.SetFrequency(frequency);
}

void FastNoiseLiteWrapper::setType(FastNoiseLite::NoiseType type) {
    noise.SetNoiseType(type);
}
