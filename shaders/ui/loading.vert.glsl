#version 330 core

layout (location = 0) in vec2 aPos;

uniform float progress;

void main()
{
    vec2 scaledPos = vec2(aPos.x * progress, aPos.y);
    gl_Position = vec4(scaledPos, 0.0, 1.0);
}
