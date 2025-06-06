#version 330 core

in vec3 fragNormal;
in vec3 fragWorldPos;
in vec3 fragViewPos;
in float fragHeight;

uniform vec3 lightDir;
uniform vec3 baseColor;

// Minimalistic earth-tone palette
const vec3 deepColor = vec3(0.22, 0.20, 0.18);    // Deep earth brown
const vec3 lowColor = vec3(0.35, 0.32, 0.28);     // Rich soil
const vec3 midColor = vec3(0.48, 0.45, 0.40);     // Light earth
const vec3 highColor = vec3(0.60, 0.58, 0.52);    // Stone/rock
const vec3 peakColor = vec3(0.78, 0.76, 0.70);    // Light stone/snow
const vec3 accentColor = vec3(0.82, 0.80, 0.75);  // Bright highlights

out vec4 FragColor;

// Hash function for noise
float hash(vec2 p) {
    vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yxz + 19.19);
    return fract((p3.x + p3.y) * p3.z);
}

// Stylized noise pattern for terrain texture
float stylizedNoise(vec2 p) {
    float n = hash(floor(p * 5.0));  // Reduced frequency
    return mix(0.97, 1.03, n);       // Very subtle variation
}

// Get color based on height with minimalistic transitions
vec3 getHeightColor(float height) {
    float h = height * 0.08; // Adjusted scale for more subtle transitions
    
    // Add very subtle position-based variation
    float variation = hash(fragWorldPos.xz * 0.02) * 0.05;
    h += variation;
    
    // Smoother transitions between colors
    if (h < -0.4) return deepColor;
    else if (h < 0.0) return mix(deepColor, lowColor, smoothstep(-0.4, 0.0, h));
    else if (h < 0.4) return mix(lowColor, midColor, smoothstep(0.0, 0.4, h));
    else if (h < 0.8) return mix(midColor, highColor, smoothstep(0.4, 0.8, h));
    else return mix(highColor, peakColor, smoothstep(0.8, 1.2, h));
}

// Cel-shading function with very subtle steps
float celShade(float value, int levels) {
    float scaled = value * float(levels);
    float floored = floor(scaled);
    float smoothed = smoothstep(0.48, 0.52, fract(scaled));
    return mix(floored, floored + 1.0, smoothed) / float(levels);
}

void main()
{
    // Normalize vectors
    vec3 norm = normalize(fragNormal);
    vec3 light = normalize(lightDir);
    
    // Subtle lighting with minimal cel-shading
    float ndotl = max(dot(norm, light), 0.0);
    float lighting = celShade(ndotl, 3); // Reduced to 3 levels for minimalism
    
    // Get base color from height
    vec3 terrainColor = getHeightColor(fragHeight);
    
    // Add very subtle noise pattern
    vec2 staticUV = fragWorldPos.xz * 0.1;
    float pattern = stylizedNoise(staticUV);
    
    // Subtle edge detection
    float edge = 1.0 - max(dot(normalize(fragViewPos), norm), 0.0);
    edge = smoothstep(0.7, 0.9, edge);
    
    // Combine everything with subtle transitions
    vec3 finalColor = terrainColor * (lighting * 0.6 + 0.4) * pattern;
    
    // Very subtle edge highlight
    finalColor = mix(finalColor, accentColor, edge * 0.15);
    
    // Subtle ambient occlusion in valleys
    float ao = smoothstep(-1.0, 1.0, fragHeight * 0.05);
    finalColor *= mix(0.92, 1.0, ao);
    
    FragColor = vec4(finalColor, 1.0);
}
