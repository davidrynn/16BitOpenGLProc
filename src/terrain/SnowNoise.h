#pragma once

#include <memory>
#include "BaseNoise.h"

class SnowNoise : public BaseNoise
{
public:
    explicit SnowNoise(std::shared_ptr<BaseNoise> baseNoise);

    float getNoise(float x, float z) const override;

private:
    std::shared_ptr<BaseNoise> baseNoise;
};