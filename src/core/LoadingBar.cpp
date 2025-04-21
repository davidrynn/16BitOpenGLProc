#include "LoadingBar.h"
#include <iostream> 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

LoadingBar::LoadingBar(const char* vertexPath, const char* fragmentPath)
    : VAO(0), VBO(0), EBO(0), progress(0.0f), shader(nullptr),
      vertexPath(vertexPath), fragmentPath(fragmentPath) {}

      void LoadingBar::initialize() {
        // Load the shader program
        shader = new Shader(vertexPath, fragmentPath);
    
        // Define a simple quad (2 triangles) that will shrink horizontally using `progress`
        // float vertices[] = {
        //     // positions (x, y)
        //     -1.0f, -0.05f,   // left bottom
        //     -1.0f,  0.05f,   // left top
        //      1.0f,  0.05f,   // right top
        //      1.0f, -0.05f    // right bottom
        // };
        float vertices[] = {
            -0.5f,  0.1f,  // far left, near bottom
            -0.5f, -0.1f,  // far right
             0.5f, -0.1f,  // top right
             0.5f,  0.1f   // top left
        };
    
        unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3
        };
    
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
    
        glBindVertexArray(VAO);
    
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
        // Link vertex attribute location 0 to aPos (vec2)
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    
        glBindVertexArray(0);
    }
    

    void LoadingBar::render(float progress, GLFWwindow* window) {
        // Store progress in case you want it later
        this->progress = progress;
    
        // Get framebuffer size and set viewport
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
    
        // Clear screen to dark gray
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);
    
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "GL error after glClear: " << err << std::endl;
        }
    
        // Use loading shader
        shader->use();
    
        err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "GL error after shader->use(): " << err << std::endl;
        }
    
        // Optional: pass progress uniform (even though unused for now)
        shader->setFloat("progress", progress);
    
        err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "GL error after setFloat: " << err << std::endl;
        }
    
        // Bind and draw fullscreen quad (VBO should already be set up)
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    
        err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "GL error after drawElements: " << err << std::endl;
        }
    }
    
    

void LoadingBar::cleanup() {
    delete shader;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
