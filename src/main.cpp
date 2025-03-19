#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream> 
#include "WindowManager.h"
#include "Shader.h"
#include "Terrain.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

// Camera settings
glm::vec3 cameraPos = glm::vec3(0.0f, 0.5f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw = -90.0f, pitch = 0.0f;
float lastX = 400, lastY = 300;
bool firstMouse = true;

// Shader sources
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 view;
    uniform mat4 projection;
    void main() {
        gl_Position = projection * view * vec4(aPos, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(0.8, 0.0, 0.6, 1.0);
    }
)";

// Function declarations
void updateCamera(GLFWwindow* window);
void setupOpenGL(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO, std::vector<unsigned int>& terrainIndices);

int main() {
    // Initialize window
    GLFWwindow* window = WindowManager::createWindow(800, 600, "16BitCraft - OpenGL");
    if (!window) return -1;

    // Hide and capture the mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, WindowManager::mouseCallback);

    // Initialize OpenGL resources
    unsigned int VAO, VBO, EBO;
    std::vector<unsigned int> terrainIndices;
    setupOpenGL(VAO, VBO, EBO, terrainIndices);

    // Shader setup
    Shader shader(vertexShaderSource, fragmentShaderSource);

    // Projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    unsigned int projLoc = glGetUniformLocation(shader.ID, "projection");
    glUseProgram(shader.ID);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        updateCamera(window); // Handle input & update camera

        // Update view matrix
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
        glUseProgram(shader.ID);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // Render
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, terrainIndices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Update camera based on user input
void updateCamera(GLFWwindow* window) {
    WindowManager::processInput(window, cameraPos, cameraFront);
}

void setupOpenGL(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO, std::vector<unsigned int>& terrainIndices) {
    const int GRID_SIZE = 50;
    std::vector<float> terrainVertices = Terrain::generateTerrain(GRID_SIZE);
    terrainIndices = Terrain::generateIndices(GRID_SIZE);

    std::cout << "DEBUG: Generated " << terrainVertices.size() / 3 << " terrain vertices.\n";
    std::cout << "DEBUG: Generated " << terrainIndices.size() / 3 << " terrain triangles.\n";

    if (terrainVertices.empty()) {
        std::cerr << "ERROR: No terrain vertices generated!\n";
        return;
    }
    if (terrainIndices.empty()) {
        std::cerr << "ERROR: No terrain indices generated!\n";
        return;
    }

    // Ensure OpenGL context is available
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "ERROR: Failed to initialize GLEW: " << glewGetErrorString(err) << "\n";
        return;
    }

    // Generate OpenGL buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    std::cout << "DEBUG: VAO, VBO, and EBO generated successfully.\n";

    // Ensure buffers were created
    if (VAO == 0 || VBO == 0 || EBO == 0) {
        std::cerr << "ERROR: Failed to generate OpenGL buffers!\n";
        return;
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    std::cout << "DEBUG: Binding VBO...\n";
    glBufferData(GL_ARRAY_BUFFER, terrainVertices.size() * sizeof(float), terrainVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    std::cout << "DEBUG: Binding EBO...\n";
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, terrainIndices.size() * sizeof(unsigned int), terrainIndices.data(), GL_STATIC_DRAW);

    std::cout << "DEBUG: Configuring VAO...\n";
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
