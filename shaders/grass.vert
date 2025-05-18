#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 instanceOffset;

uniform mat4 view;
uniform mat4 projection;
uniform float time;

flat out float instanceID;

void main()
{
    instanceID = float(gl_InstanceID);
    vec3 pos = aPos;
    // sway intensity increases with height (aPos.y)
    float sway = sin(time * 3.0 + instanceOffset.x * 10.0) * 0.02;
    pos.x += sway * aPos.y;  // ← low y = little sway, high y = full sway

    gl_Position = projection * view * vec4(pos + instanceOffset, 1.0);
}
