#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform vec3 instanceOffset;

void main()
{
    vec3 pos = aPos;
    pos.x += sin(time * 3.0 + instanceOffset.x * 10.0) * 0.01; // sway
    gl_Position = projection * view * model * vec4(pos + instanceOffset, 1.0);
}