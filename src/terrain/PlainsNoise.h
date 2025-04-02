#include "NoiseFunction.h"

class PlainsNoise : public NoiseFunction {
public:
    float getHeight(float x, float z) const override;
};