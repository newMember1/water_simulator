#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out VS_OUT
{
    vec3 normal;
} vs_out;

struct wave
{
    float A;
    float omega;
    float phi;
    vec2 Dxy;
};

uniform wave waves[6];
uniform float maxWaves;
uniform float time;

void main(void)
{
    vec3 pos = aPos;
    for(int i=0;i<maxWaves;++i)
    {
        pos.z += waves[i].A * sin((pos.x * waves[i].Dxy.x + pos.y * waves[i].Dxy.y) * waves[i].omega + time * waves[i].phi);
    }

    vec3 normal=vec3(0,0,1);
    for(int i=0;i<maxWaves;++i)
    {
        normal.x -= waves[i].A * waves[i].Dxy.x * waves[i].omega * cos((pos.x * waves[i].Dxy.x + pos.y * waves[i].Dxy.y) * waves[i].omega + waves[i].phi * time);
        normal.y -= waves[i].A * waves[i].Dxy.y * waves[i].omega * cos((pos.x * waves[i].Dxy.x + pos.y * waves[i].Dxy.y) * waves[i].omega + waves[i].phi * time);
    }

    normal = vec3(projection * view * model * vec4(normal,1.0));
    normal = normalize(normal);
    vs_out.normal = normal;
    gl_Position = projection * view * model * vec4(pos,1.0);
}
