#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//simple sin wave
//need to consider some wave direction
struct wave
{
    float A;
    float omega;
    float phi;
    vec2 Dxy;
};

uniform float time;
out vec3 originNormal;
out vec3 originPos;

void main(void)
{
    wave waves[4];
    waves[0].A = 0.1;
    waves[0].Dxy.x = 4;
    waves[0].Dxy.y = -3;
    waves[0].omega = 1;
    waves[0].phi = 1;

    waves[1].A = 0.4;
    waves[1].Dxy.x = 4;
    waves[1].Dxy.y = -5;
    waves[1].omega = 2;
    waves[1].phi = 1;

    waves[2].A = 0.5;
    waves[2].Dxy.x = -1;
    waves[2].Dxy.y = 3;
    waves[2].omega = 2;
    waves[2].phi = 1;

    waves[3].A = 0.1;
    waves[3].Dxy.x = -2;
    waves[3].Dxy.y = 6;
    waves[3].omega = 2;
    waves[3].phi = 1;
    for(int i=0;i<4;++i)
    {
        waves[i].Dxy = normalize(waves[i].Dxy);
    }

    vec3 pos = aPos;
    for(int i=0;i<4;++i)
    {
        pos.y += waves[i].A / 5 * sin((pos.x * waves[i].Dxy.x + pos.y * waves[i].Dxy.y) * waves[i].omega + time * waves[i].phi);
    }

    vec3 normal=vec3(0,0,1);
    for(int i=0;i<4;++i)
    {
        normal.x -= waves[i].A / 5 * waves[i].Dxy.x * waves[i].omega * cos((pos.x * waves[i].Dxy.x + pos.y * waves[i].Dxy.y) * waves[i].omega + waves[i].phi * time);
        normal.y -= waves[i].A / 5 * waves[i].Dxy.y * waves[i].omega * cos((pos.x * waves[i].Dxy.x + pos.y * waves[i].Dxy.y) * waves[i].omega + waves[i].phi * time);
    }

    normalize(normal);
    originNormal = normal;
    originPos = pos;

    normal = vec3(projection * view * model * vec4(normal,1.0));
    gl_Position = projection * view * model * vec4(pos,1.0);
}
