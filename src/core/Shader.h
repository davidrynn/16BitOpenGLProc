#ifndef SHADER_H
#define SHADER_H

#include <string>        // ✅ Standard library first
#include <GL/glew.h>     // ✅ GLEW immediately after standard headers
#include <glm/glm.hpp>   // ✅ Other external libraries after GLEW

class Shader {
public:
    unsigned int ID;
    Shader(const char* vertexSource, const char* fragmentSource);
    void use();
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setFloat(const std::string &name, float value) const;
    
    ~Shader();

private:
    void checkCompileErrors(unsigned int shader, std::string type); // ✅ Useful for debugging
};

#endif
