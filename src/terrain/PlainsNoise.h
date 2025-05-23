#pragma once

#include <memory>
#include "BaseNoise.h"

class PlainsNoise : public BaseNoise
{
public:
    explicit PlainsNoise(std::shared_ptr<BaseNoise> baseNoise);

    float getNoise(float x, float z) const override;

private:
    std::shared_ptr<BaseNoise> baseNoise;
};
