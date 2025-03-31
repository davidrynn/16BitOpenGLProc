#version 330 core
in vec3 vPosition;
out vec4 FragColor;

void main() {
    float height = normalize(vPosition).y;
    vec3 topColor = vec3(0.3, 0.6, 0.95);
    vec3 bottomColor = vec3(0.9, 0.9, 1.0);
    vec3 color = mix(bottomColor, topColor, height * 0.5 + 0.5);
    FragColor = vec4(color, 1.0);
}