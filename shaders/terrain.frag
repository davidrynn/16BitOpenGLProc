#version 330 core

in vec3 fragNormal;

uniform vec3 lightDir;
uniform vec3 baseColor;

out vec4 FragColor;

void main()
{
vec3 norm = normalize(fragNormal);
vec3 light = normalize(lightDir);
float lighting = max(dot(norm, light), 0.0);
vec3 finalColor = baseColor * lighting;
FragColor = vec4(finalColor, 1.0);
}
