#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 originNormal;
out vec3 originPos;

void main()
{
    originPos = pos;
    originNormal = normalize(normal);
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
