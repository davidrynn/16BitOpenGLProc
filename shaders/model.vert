#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 aWeights;
layout (location = 4) in ivec4 aJoints;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 bones[100];
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    
    // Calculate skinned position
    vec4 skinnedPosition = vec4(0.0);
    for (int i = 0; i < 4; ++i) {
        if (aJoints[i] >= 0) {
            skinnedPosition += bones[aJoints[i]] * vec4(aPos, 1.0) * aWeights[i];
        }
    }
    
    // If no valid joints/weights, use original position
    if (skinnedPosition == vec4(0.0)) {
        skinnedPosition = vec4(aPos, 1.0);
    }
    
    // Transform the skinned position
    FragPos = vec3(model * skinnedPosition);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * model * skinnedPosition;
} 