#pragma once

#include <glad/glad.h>
#include <functional>

template<GLenum ResourceType>
class GLResource {
public:
    GLResource() : id(0) {
        glGenBuffers(1, &id);
    }

    ~GLResource() {
        if (id != 0) {
            glDeleteBuffers(1, &id);
        }
    }

    // Prevent copying
    GLResource(const GLResource&) = delete;
    GLResource& operator=(const GLResource&) = delete;

    // Allow moving
    GLResource(GLResource&& other) noexcept : id(other.id) {
        other.id = 0;
    }

    GLResource& operator=(GLResource&& other) noexcept {
        if (this != &other) {
            if (id != 0) {
                glDeleteBuffers(1, &id);
            }
            id = other.id;
            other.id = 0;
        }
        return *this;
    }

    void bind() const {
        glBindBuffer(ResourceType, id);
    }

    void unbind() const {
        glBindBuffer(ResourceType, 0);
    }

    GLuint get() const {
        return id;
    }

private:
    GLuint id;
};

// Specialized types for common buffer types
using VertexBuffer = GLResource<GL_ARRAY_BUFFER>;
using ElementBuffer = GLResource<GL_ELEMENT_ARRAY_BUFFER>;
using UniformBuffer = GLResource<GL_UNIFORM_BUFFER>;

// RAII wrapper for Vertex Array Objects
class VertexArray {
public:
    VertexArray() : id(0) {
        glGenVertexArrays(1, &id);
    }

    ~VertexArray() {
        if (id != 0) {
            glDeleteVertexArrays(1, &id);
        }
    }

    // Prevent copying
    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    // Allow moving
    VertexArray(VertexArray&& other) noexcept : id(other.id) {
        other.id = 0;
    }

    VertexArray& operator=(VertexArray&& other) noexcept {
        if (this != &other) {
            if (id != 0) {
                glDeleteVertexArrays(1, &id);
            }
            id = other.id;
            other.id = 0;
        }
        return *this;
    }

    void bind() const {
        glBindVertexArray(id);
    }

    void unbind() const {
        glBindVertexArray(0);
    }

    GLuint get() const {
        return id;
    }

private:
    GLuint id;
}; 