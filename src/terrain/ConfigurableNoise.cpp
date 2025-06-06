#include "ConfigurableNoise.h"

ConfigurableNoise::ConfigurableNoise(std::shared_ptr<BaseNoise> base, NoiseConfig cfg)
    : baseNoise(std::move(base))
    , config(std::move(cfg))
{
}

float ConfigurableNoise::getNoise(float x, float z) const
{
    float result = 0.0f;
    
    // Apply base frequency
    float baseX = x * config.baseFrequency;
    float baseZ = z * config.baseFrequency;
    
    // Generate noise for each layer
    for (const auto& layer : config.layers) {
        float frequency = layer.frequency;
        float amplitude = layer.amplitude;
        
        // Apply octaves with persistence and lacunarity
        for (int o = 0; o < layer.octaves; o++) {
            // We don't set the frequency on the noise generator directly
            // Instead, we scale our coordinates by the frequency
            float sampleX = baseX * frequency;
            float sampleZ = baseZ * frequency;
            
            result += baseNoise->getNoise(sampleX, sampleZ) * amplitude;
            
            frequency *= layer.lacunarity;
            amplitude *= layer.persistence;
        }
    }
    
    return result * config.baseAmplitude;
}

void ConfigurableNoise::setConfig(const NoiseConfig& newConfig)
{
    config = newConfig;
} 