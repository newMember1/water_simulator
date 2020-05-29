#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;

out vec3 originNormal;
out vec3 originPos;

struct gerstnerWave
{
    float A;
    float omega;
    float phi;
    vec2 Dxy;
};
gerstnerWave waves[10];

void main(void)
{
    waves[0].A = 0.3;
    waves[0].Dxy.x = 0.5;
    waves[0].Dxy.y = 0.0;
    waves[0].omega = 20;
    waves[0].phi = 1;

    waves[1].A = 0.4;
    waves[1].Dxy.x = 0.5;
    waves[1].Dxy.y = 0.7;
    waves[1].omega = 12;
    waves[1].phi = 1;

    waves[2].A = 0.6;
    waves[2].Dxy.x = 0.5;
    waves[2].Dxy.y = 0.5;
    waves[2].omega = 2;
    waves[2].phi = 1;

    waves[3].A = 0.5;
    waves[3].Dxy.x = 0.5;
    waves[3].Dxy.y = 0.8;
    waves[3].omega = 2;
    waves[3].phi = 1;
    for(int i=0;i<4;++i)
        waves[i].Dxy = normalize(waves[i].Dxy);

    vec3 offset = vec3(0);
    vec3 pos = aPos;
    for(int i=0;i<1;++i)
    {
        float A = waves[i].A;
        float omega = waves[i].omega;
        float phi = waves[i].phi;
        float Dx = waves[i].Dxy.x;
        float Dy = waves[i].Dxy.y;

        float Q = 1.0/(omega * A);
        offset.x += Q * A * Dx * cos((Dx * pos.x + Dy * pos.y) * omega + phi * time) / 5;
        offset.y += Q * A * Dy * cos((Dx * pos.x + Dy * pos.y) * omega + phi * time) / 5;
        offset.z += A * sin((Dx * pos.x + Dy * pos.y) * omega + phi * time) / 5;
    }

    vec3 normal = vec3(0,0,1);
    pos += offset;
    for(int i=0;i<1;++i)
    {
        float A = waves[i].A;
        float omega = waves[i].omega;
        float phi = waves[i].phi;
        float Dx = waves[i].Dxy.x;
        float Dy = waves[i].Dxy.y;

        float Q = 1.0/(omega * A);
        normal.x -= Dx * omega * A * cos((Dx * pos.x + Dy * pos.y) * omega + phi * time);
        normal.y -= Dy * omega * A * cos((Dx * pos.x + Dy * pos.y) * omega + phi * time);
        normal.z -= Q * omega * A * sin((Dx * pos.x + Dy * pos.y) * omega + phi * time);
    }
    normal = normalize(normal);
    originNormal = normal;
    originPos = pos;
    gl_Position = projection * view * model * vec4(pos,1.0);
}
