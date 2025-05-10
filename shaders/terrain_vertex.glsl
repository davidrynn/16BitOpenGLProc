#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragNormal;

void main()
{
    // Convert normal to world space using the normal matrix
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    fragNormal = normalize(normalMatrix * inNormal);

    gl_Position = projection * view * model * vec4(inPosition, 1.0);
}
