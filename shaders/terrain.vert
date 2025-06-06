#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragNormal;
out vec3 fragWorldPos;
out vec3 fragViewPos;
out float fragHeight;

void main()
{
    vec4 worldPos = model * vec4(inPosition, 1.0);
    fragWorldPos = worldPos.xyz;
    fragHeight = inPosition.y;  // Pass raw height
    
    // View space position for edge detection
    vec4 viewPos = view * worldPos;
    fragViewPos = viewPos.xyz;
    
    // Normal in world space
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    fragNormal = normalize(normalMatrix * inNormal);

    gl_Position = projection * viewPos;
}