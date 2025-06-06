#pragma once

#include <vector>

struct NoiseLayer {
    float frequency = 1.0f;
    float amplitude = 1.0f;
    float persistence = 0.5f; // How much each octave's amplitude decreases
    float lacunarity = 2.0f;  // How much each octave's frequency increases
    int octaves = 1;
};

struct NoiseConfig {
    float baseFrequency = 0.01f;
    float baseAmplitude = 1.0f;
    std::vector<NoiseLayer> layers;
    
    // Static factory methods for different terrain types
    static NoiseConfig Plains() {
        NoiseConfig config;
        config.baseFrequency = 0.2f;
        config.baseAmplitude = 5.0f;
        config.layers = {
            {0.2f, 5.0f, 0.5f, 2.0f, 1}  // Single layer for smooth plains
        };
        return config;
    }
    
    static NoiseConfig Mountains() {
        NoiseConfig config;
        config.baseFrequency = 0.01f;
        config.baseAmplitude = 400.0f;
        config.layers = {
            {0.01f, 400.0f, 0.5f, 2.0f, 1},  // Base mountain shape
            {0.05f, 30.0f, 0.5f, 2.0f, 1},   // Mid-scale features
            {0.1f, 10.0f, 0.5f, 2.0f, 1}     // Fine details
        };
        return config;
    }
    
    static NoiseConfig Desert() {
        NoiseConfig config;
        config.baseFrequency = 0.03f;
        config.baseAmplitude = 2.0f;
        config.layers = {
            {0.03f, 2.0f, 0.5f, 2.0f, 1},    // Base dunes
            {0.1f, 0.5f, 0.5f, 2.0f, 1}      // Small ripples
        };
        return config;
    }
    
    static NoiseConfig Snow() {
        NoiseConfig config;
        config.baseFrequency = 0.03f;
        config.baseAmplitude = 2.0f;
        config.layers = {
            {0.03f, 2.0f, 0.5f, 2.0f, 1},    // Base snow fields
            {0.08f, 0.3f, 0.5f, 2.0f, 1}     // Snow drifts
        };
        return config;
    }
}; 