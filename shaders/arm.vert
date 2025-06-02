#version 300 es
precision mediump float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 baseColor;

out vec3 FragColor;

void main()
{
    // Basic directional light from upper-right
    vec3 lightDir = normalize(vec3(1.0, 1.0, 0.5));
    
    // Transform normal to world space
    vec3 normal = normalize(mat3(model) * aNormal);
    
    // Calculate diffuse lighting
    float diff = max(dot(normal, lightDir), 0.2); // 0.2 is ambient light
    
    // Pass color to fragment shader
    FragColor = baseColor * diff;
    
    // Transform position
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
