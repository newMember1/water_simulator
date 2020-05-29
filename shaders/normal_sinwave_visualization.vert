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
    float direcX;
    float direcY;
};

uniform float time;

void main(void)
{
    wave waves[4];
    waves[0].A = 0.1;
    waves[0].direcX = 4;
    waves[0].direcY = -3;
    waves[0].omega = 1;
    waves[0].phi = 1;

    waves[1].A = 0.4;
    waves[1].direcX = 4;
    waves[1].direcY = -5;
    waves[1].omega = 2;
    waves[1].phi = 1;

    waves[2].A = 0.5;
    waves[2].direcX = -1;
    waves[2].direcY = 3;
    waves[2].omega = 2;
    waves[2].phi = 1;

    waves[3].A = 0.1;
    waves[3].direcX = -2;
    waves[3].direcY = 6;
    waves[3].omega = 2;
    waves[3].phi = 1;


    vec3 pos = aPos;
    for(int i=0;i<4;++i)
    {
        pos.y += waves[i].A / 8 * sin((pos.x * waves[i].direcX + pos.y * waves[i].direcY) * waves[i].omega + time);
    }

    vec3 normal=vec3(0,0,1);
    for(int i=0;i<4;++i)
    {
        normal.x -= waves[i].A / 8 * waves[i].direcX * waves[i].omega * cos((pos.x * waves[i].direcX + pos.y * waves[i].direcY) * waves[i].omega + waves[i].phi * time);
        normal.y -= waves[i].A / 8 * waves[i].direcY * waves[i].omega * cos((pos.x * waves[i].direcX + pos.y * waves[i].direcY) * waves[i].omega + waves[i].phi * time);
    }

    normal = vec3(projection * view * model * vec4(normal,1.0));
    normalize(normal);
    vs_out.normal = normal;
    gl_Position = projection * view * model * vec4(pos,1.0);
}
