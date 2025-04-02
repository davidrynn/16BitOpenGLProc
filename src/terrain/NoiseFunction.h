// NoiseFunction.h
#ifndef NOISE_FUNCTION_H
#define NOISE_FUNCTION_H

class NoiseFunction {
public:
    virtual ~NoiseFunction() = default;
    virtual float getHeight(float x, float z) const = 0;
};

#endif
