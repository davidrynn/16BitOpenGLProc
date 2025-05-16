#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

class LoadingBar {
public:
    LoadingBar(const char* vertexPath, const char* fragmentPath);
    void initialize();
    void render(float progress, GLFWwindow* window);
    void cleanup();

private:
    unsigned int VAO, VBO, EBO;
    Shader* shader;
    float progress;

    const char* vertexPath;
    const char* fragmentPath;
};