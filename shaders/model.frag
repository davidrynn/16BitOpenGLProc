#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D texture_diffuse;

void main()
{
    // Base color (purple)
    vec3 color = vec3(0.4, 0.2, 0.6);
    
    // Simple lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    
    // Combine ambient and diffuse
    vec3 result = (0.3 + diff * 0.7) * color;
    
    FragColor = vec4(result, 1.0);
} 