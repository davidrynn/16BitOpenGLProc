#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 vPosition;
out vec3 normal;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vPosition = aPos;
    vec4 pos = projection * view * vec4(aPos * 100.0, 1.0); // large cube
    gl_Position = pos.xyww;
}