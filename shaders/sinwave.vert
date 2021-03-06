#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//simple sin wave
struct wave
{
    float A;
    float omega;
    float phi;
    vec2 Dxy;
};

uniform wave waves[6];
uniform int maxWaves;
uniform float time;

out vec3 originNormal;
out vec3 originPos;

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

    normal = normalize(normal);
    originNormal = normal;
    originPos = pos;

    gl_Position = projection * view * model * vec4(pos,1.0);
}
