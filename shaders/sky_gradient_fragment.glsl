#version 330 core
in vec2 vUV;
out vec4 FragColor;

void main() {
    vec3 topColor = vec3(0.3, 0.6, 0.95);      // Sky blue
    vec3 bottomColor = vec3(0.9, 0.9, 1.0);    // Horizon color
    vec3 color = mix(bottomColor, topColor, vUV.y);
    FragColor = vec4(color, 1.0);
}
