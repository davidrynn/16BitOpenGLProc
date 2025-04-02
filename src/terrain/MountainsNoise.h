// MountainsNoise.h
#include "NoiseFunction.h"

class MountainsNoise : public NoiseFunction {
public:
    float getHeight(float x, float z) const override;
};