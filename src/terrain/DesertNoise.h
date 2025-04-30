#pragma once

#include <memory>
#include "BaseNoise.h"

class DesertNoise : public BaseNoise
{
public:
    explicit DesertNoise(std::shared_ptr<BaseNoise> baseNoise);

    float getNoise(float x, float z) const override;

private:
    std::shared_ptr<BaseNoise> baseNoise;
};
