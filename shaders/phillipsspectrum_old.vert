#version 330 core
/*
*statistic model
*in this model what we need are
*1.Lx,Ly which define the size of the water
*2.N,M which define the partition number in x and y direction
*3.Dw and Vw define the direction of wind and the speed
*4.A the amplitude
*5.Er and Ei, two random variable
*D(x,t) for wave tip
*calculate normal N
*/
const float PI = 3.1415926;
const float g = 9.8;
const float e = 2.7182818;
const float epslion = 1e-6;

layout(location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int N = 50;
const vec2 Dw = vec2(0.4, 0.5);
const float Vw = 1.5;
const float A = 0.1;
const float Er = 0.1;
const float Ei = 0.3;
uniform float time;

out vec3 originPos;
out vec3 originNormal;

void main(void)
{
    vec3 p = pos;
    float delta = 2 / N;

    vec2 h = vec2(0);
    vec3 normal = vec3(0);
    vec2 d = vec2(0);

    for(int i=0;i<N;++i)
        for(int j=0;j<N;++j)
        {
            float n = i - N/2;
            float m = j - N/2;
            vec2 k = vec2(n,m) * 2 * PI / 3;
            float lenK = length(k);
            float k4 = pow(lenK, 4);
            float kDotX = dot(k, p.xy);

            float L = Vw * Vw / g;
            float kDotW = dot(k, Dw);
            float p = A * pow(e, -1/(lenK * lenK * L * L)) / k4 * kDotW * kDotW;
            vec2 h0 = vec2(Er, Ei) * sqrt(p / 2);
            vec2 conH0 = vec2(h0.x, -h0.y);
            float wK = sqrt(g * lenK);
            vec2 eI = vec2(cos(wK * time), sin(wK * time));
            vec2 conEI = vec2(-eI.y, eI.x);
            //h(k,t)
            vec2 hK = vec2(h0.x * eI.x - h0.y * eI.y, h0.x * eI.y + h0.y * eI.x) +
                    vec2(conH0.x * conEI.x - conH0.y * conEI.y, conH0.x * conEI.y + conH0.y * conEI.x);

            //e^(ikx)
            vec2 eIKX = vec2(cos(kDotX), sin(kDotX));
            //H(x,t) = h(k,t) * e^(ikx)
            vec2 HXT = vec2(hK.x * eIKX.x - hK.y * eIKX.y, hK.x * eIKX.y + hK.y * eIKX.x);
            h += HXT;
            //normal
            vec3 t = vec3(-HXT.y * k.x, HXT.x * k.y, 1.0);
            normal += vec3(-t.x, -t.y, 1.0);

            //calculate D(x,y)
            vec2 dXT = vec2(HXT.y, -HXT.x) * k / lenK;
            d += dXT;
        }

    p += vec3(d.x, d.y, h.x);
    originPos = p;
    originNormal = normalize(normal);

    gl_Position = projection * view * model * vec4(p, 1.0);
}
