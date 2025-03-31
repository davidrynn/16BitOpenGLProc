#version 330 core
in vec3 fragPos;
in vec3 normal;

out vec4 FragColor;

uniform vec3 lightDir;  // normalized direction of the fake light
uniform vec3 baseColor; // base terrain color

void main()
{
    // Normalize normal and lightDir just in case
    vec3 n = normalize(normal);
    vec3 l = normalize(lightDir);

    // Fake light intensity from dot product (clamped to avoid negatives)
    float light = clamp(dot(n, l), 0.4, 1.0); // 0.2 is min ambient light

    vec3 color = baseColor * light;
    FragColor = vec4(color, 1.0);
}
