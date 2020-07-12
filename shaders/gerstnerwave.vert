#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

struct wave
{
    float A;
    float omega;
    float phi;
    vec2 Dxy;
};
uniform wave waves[10];
uniform int maxWaves;
uniform float time;

out vec3 originNormal;
out vec3 originPos;
void main(void)
{
    vec3 offset = vec3(0);
    vec3 pos = aPos;
    for(int i=0;i<maxWaves;++i)
    {
        float A = waves[i].A;
        float omega = waves[i].omega;
        float phi = waves[i].phi;
        float Dx = waves[i].Dxy.x;
        float Dy = waves[i].Dxy.y;

        float Q = 1.0/(omega * A)/maxWaves;
        offset.x += Q * A * Dx * cos((Dx * pos.x + Dy * pos.y) * omega + phi * time);
        offset.y += Q * A * Dy * cos((Dx * pos.x + Dy * pos.y) * omega + phi * time);
        offset.z += A * sin((Dx * pos.x + Dy * pos.y) * omega + phi * time);
    }

    vec3 normal = vec3(0,0,1);
    pos += offset;
    for(int i=0;i<maxWaves;++i)
    {
        float A = waves[i].A;
        float omega = waves[i].omega;
        float phi = waves[i].phi;
        float Dx = waves[i].Dxy.x;
        float Dy = waves[i].Dxy.y;

        float Q = 1.0/(omega * A)/maxWaves;
        normal.x -= Dx * omega * A * cos((Dx * pos.x + Dy * pos.y) * omega + phi * time);
        normal.y -= Dy * omega * A * cos((Dx * pos.x + Dy * pos.y) * omega + phi * time);
        normal.z -= Q * omega * A * sin((Dx * pos.x + Dy * pos.y) * omega + phi * time);
    }
    normal = normalize(normal);
    originNormal = normal;
    originPos = pos;
    gl_Position = projection * view * model * vec4(pos,1.0);
}
