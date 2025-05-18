#version 330 core

flat in float instanceID;
out vec4 FragColor;

void main()
{
    // Pseudo-random variation per blade
    float variation = fract(sin(instanceID * 13.37) * 43758.5453);

    // ↓ Darker green range
    float green = 0.4 + variation * 0.2;   // 0.4–0.6

    // ↓ Less red (darker yellow tint)
    float red = 0.05 + variation * 0.2;    // 0.05–0.25

    // ↓ Less blue (darker cyan tint)
    float blue = 0.3 - variation * 0.2;    // 0.3–0.1

    FragColor = vec4(red, green, blue, 1.0);
}
