#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "tiny_gltf.h"
#include "Model.h"
#include "Debug.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
    : vertices(vertices), indices(indices), textureId(0)
{
    setupMesh();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

    // Texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoord));

    // Bone weights
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, weights));

    // Bone indices (using integers)
    glEnableVertexAttribArray(4);
    glVertexAttribIPointer(4, 4, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, joints));

    glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader)
{
    Debug::log("Drawing mesh with " + std::to_string(vertices.size()) + " vertices and " + 
               std::to_string(indices.size()) + " indices");

    if (textureId > 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        shader.setInt("texture_diffuse", 0);
        Debug::log("Using texture ID: " + std::to_string(textureId));
    }
    else
    {
        Debug::log("No texture bound for this mesh");
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    
    // Check for OpenGL errors
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        Debug::logError("OpenGL error during mesh draw: 0x" + std::to_string(err));
    }
    
    glBindVertexArray(0);
}

Model::Model(const char *path)
{
    loadModel(path);
}

Model::~Model() = default;

void Model::Draw(Shader &shader)
{
    for (const auto &mesh : meshes)
    {
        mesh->Draw(shader);
    }
}

void Model::loadModel(const std::string &path)
{
    tinygltf::TinyGLTF loader;
    gltf = std::make_unique<tinygltf::Model>();

    std::string err, warn;
    bool ret;

    Debug::log("Loading model from: " + path);

    // Check file extension
    std::string ext = path.substr(path.find_last_of(".") + 1);
    if (ext == "glb")
    {
        ret = loader.LoadBinaryFromFile(gltf.get(), &err, &warn, path);
    }
    else
    {
        ret = loader.LoadASCIIFromFile(gltf.get(), &err, &warn, path);
    }

    if (!warn.empty())
    {
        Debug::logWarning("GLTF Warning: " + warn);
    }

    if (!err.empty())
    {
        Debug::logError("GLTF Error: " + err);
    }

    if (!ret)
    {
        Debug::logError("Failed to load GLTF file: " + path);
        return;
    }

    Debug::log("Model loaded successfully. Processing nodes...");
    Debug::log("Number of scenes: " + std::to_string(gltf->scenes.size()));
    Debug::log("Number of meshes: " + std::to_string(gltf->meshes.size()));

    // Process all nodes
    const tinygltf::Scene &scene = gltf->scenes[gltf->defaultScene];
    Debug::log("Number of nodes in default scene: " + std::to_string(scene.nodes.size()));
    
    for (int nodeIndex : scene.nodes)
    {
        processNode(gltf->nodes[nodeIndex], *gltf);
    }

    Debug::log("Model processing complete. Created " + std::to_string(meshes.size()) + " meshes.");
}

void Model::processNode(const tinygltf::Node &node, const tinygltf::Model &model)
{
    if (node.mesh >= 0)
    {
        meshes.push_back(processMesh(model.meshes[node.mesh], model));
    }

    for (int childIndex : node.children)
    {
        processNode(model.nodes[childIndex], model);
    }
}

std::unique_ptr<Mesh> Model::processMesh(const tinygltf::Mesh &mesh, const tinygltf::Model &model)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    Debug::log("Processing mesh with " + std::to_string(mesh.primitives.size()) + " primitives");

    for (const auto &primitive : mesh.primitives)
    {
        const float *positionBuffer = nullptr;
        const float *normalBuffer = nullptr;
        const float *texcoordBuffer = nullptr;
        const float *weightsBuffer = nullptr;
        const uint16_t *jointsBuffer = nullptr;
        size_t vertexCount = 0;

        // Get position data
        if (primitive.attributes.find("POSITION") != primitive.attributes.end())
        {
            const tinygltf::Accessor &accessor = model.accessors[primitive.attributes.at("POSITION")];
            const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
            positionBuffer = reinterpret_cast<const float *>(&model.buffers[bufferView.buffer].data[bufferView.byteOffset + accessor.byteOffset]);
            vertexCount = accessor.count;
            Debug::log("Found " + std::to_string(vertexCount) + " vertices");
        }
        else
        {
            Debug::logError("No position data found in mesh!");
        }

        // Get normal data
        if (primitive.attributes.find("NORMAL") != primitive.attributes.end())
        {
            const tinygltf::Accessor &accessor = model.accessors[primitive.attributes.at("NORMAL")];
            const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
            normalBuffer = reinterpret_cast<const float *>(&model.buffers[bufferView.buffer].data[bufferView.byteOffset + accessor.byteOffset]);
        }

        // Get texcoord data
        if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end())
        {
            const tinygltf::Accessor &accessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
            const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
            texcoordBuffer = reinterpret_cast<const float *>(&model.buffers[bufferView.buffer].data[bufferView.byteOffset + accessor.byteOffset]);
        }

        // Get weights data
        if (primitive.attributes.find("WEIGHTS_0") != primitive.attributes.end())
        {
            const tinygltf::Accessor &accessor = model.accessors[primitive.attributes.at("WEIGHTS_0")];
            const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
            weightsBuffer = reinterpret_cast<const float *>(&model.buffers[bufferView.buffer].data[bufferView.byteOffset + accessor.byteOffset]);
        }

        // Get joints data
        if (primitive.attributes.find("JOINTS_0") != primitive.attributes.end())
        {
            const tinygltf::Accessor &accessor = model.accessors[primitive.attributes.at("JOINTS_0")];
            const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
            jointsBuffer = reinterpret_cast<const uint16_t *>(&model.buffers[bufferView.buffer].data[bufferView.byteOffset + accessor.byteOffset]);
        }

        // Combine all attributes into vertices
        for (size_t i = 0; i < vertexCount; ++i)
        {
            Vertex vertex;
            
            if (positionBuffer)
            {
                vertex.position = glm::vec3(
                    positionBuffer[i * 3],
                    positionBuffer[i * 3 + 1],
                    positionBuffer[i * 3 + 2]
                );
            }

            if (normalBuffer)
            {
                vertex.normal = glm::vec3(
                    normalBuffer[i * 3],
                    normalBuffer[i * 3 + 1],
                    normalBuffer[i * 3 + 2]
                );
            }

            if (texcoordBuffer)
            {
                vertex.texCoord = glm::vec2(
                    texcoordBuffer[i * 2],
                    texcoordBuffer[i * 2 + 1]
                );
            }

            if (weightsBuffer)
            {
                vertex.weights = glm::vec4(
                    weightsBuffer[i * 4],
                    weightsBuffer[i * 4 + 1],
                    weightsBuffer[i * 4 + 2],
                    weightsBuffer[i * 4 + 3]
                );
            }

            if (jointsBuffer)
            {
                vertex.joints = glm::ivec4(
                    static_cast<int>(jointsBuffer[i * 4]),
                    static_cast<int>(jointsBuffer[i * 4 + 1]),
                    static_cast<int>(jointsBuffer[i * 4 + 2]),
                    static_cast<int>(jointsBuffer[i * 4 + 3])
                );
            }

            vertices.push_back(vertex);
        }

        // Get indices data
        if (primitive.indices >= 0)
        {
            const tinygltf::Accessor &accessor = model.accessors[primitive.indices];
            const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
            const void *data = &model.buffers[bufferView.buffer].data[bufferView.byteOffset + accessor.byteOffset];
            
            size_t indexCount = accessor.count;
            Debug::log("Found " + std::to_string(indexCount) + " indices");
            
            // Convert indices to uint32_t
            switch (accessor.componentType)
            {
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                {
                    const uint16_t *buf = static_cast<const uint16_t *>(data);
                    for (size_t i = 0; i < indexCount; ++i)
                        indices.push_back(static_cast<uint32_t>(buf[i]));
                    break;
                }
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                {
                    const uint32_t *buf = static_cast<const uint32_t *>(data);
                    for (size_t i = 0; i < indexCount; ++i)
                        indices.push_back(buf[i]);
                    break;
                }
                default:
                    Debug::logError("Index component type " + std::to_string(accessor.componentType) + " not supported!");
                    break;
            }
        }
        else
        {
            Debug::logError("No indices found in mesh!");
        }
    }

    auto mesh_ptr = std::make_unique<Mesh>(vertices, indices);

    // Load material/texture if available
    if (!mesh.primitives.empty() && mesh.primitives[0].material >= 0)
    {
        const auto &material = model.materials[mesh.primitives[0].material];
        if (material.pbrMetallicRoughness.baseColorTexture.index >= 0)
        {
            mesh_ptr->textureId = loadTexture(model, material.pbrMetallicRoughness.baseColorTexture.index);
        }
    }

    return mesh_ptr;
}

uint32_t Model::loadTexture(const tinygltf::Model &model, int textureIndex)
{
    const tinygltf::Texture &texture = model.textures[textureIndex];
    const tinygltf::Image &image = model.images[texture.source];

    uint32_t textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    GLenum format = GL_RGBA;
    if (image.component == 3)
        format = GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format, GL_UNSIGNED_BYTE, image.image.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureId;
}