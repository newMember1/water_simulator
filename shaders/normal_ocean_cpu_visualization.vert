#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
    vec3 normal;
} vs_out;

void main(void)
{
    vs_out.normal = normalize(normal);
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
