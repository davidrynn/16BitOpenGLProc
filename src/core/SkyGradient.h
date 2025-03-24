#ifndef SKYGRADIENT_H
#define SKYGRADIENT_H

#include <GL/glew.h>
#include "Shader.h"

class SkyGradient {
public:
    SkyGradient();
    ~SkyGradient();
    void render();

private:
    GLuint VAO, VBO;
    Shader* shader;
    void setupQuad();
};

#endif
