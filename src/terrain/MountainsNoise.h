#pragma once

#include <memory>
#include "BaseNoise.h"

class MountainsNoise : public BaseNoise
{
public:
    explicit MountainsNoise(std::shared_ptr<BaseNoise> baseNoise);

    float getNoise(float x, float z) const override;

private:
    std::shared_ptr<BaseNoise> baseNoise;
};
