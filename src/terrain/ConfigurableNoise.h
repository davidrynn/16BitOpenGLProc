#pragma once

#include "BaseNoise.h"
#include "NoiseConfig.h"
#include <memory>

class ConfigurableNoise : public BaseNoise {
public:
    explicit ConfigurableNoise(std::shared_ptr<BaseNoise> baseNoise, NoiseConfig config);

    float getNoise(float x, float z) const override;
    void setConfig(const NoiseConfig& newConfig);
    const NoiseConfig& getConfig() const { return config; }

private:
    std::shared_ptr<BaseNoise> baseNoise;
    NoiseConfig config;
}; 