#version 330 core

/*
* adapt from seacape.shader
* github.com/tdmaav/shadertoy/blob/master/Seascape.shader
*/
out vec4 fColor;
in vec3 originNormal;
in vec3 originPos;

const float PI = 3.1415926;

vec3 viewPos = vec3(0, -3, 2);
vec3 lightDirec = vec3(0.0, 0.8, 1.0);

vec3 seaBase = vec3(0.0, 0.09, 0.18);
vec3 seaWaterColor = vec3(0.8, 0.9, 0.6) * 0.6;

float diffuse(vec3 normal, vec3 lightDir, float p)
{
    return pow(dot(normal, lightDir) * 0.4 + 0.6, p);
}

float specular(vec3 normal, vec3 lightDir, vec3 viewDir, float s)
{
    float nrm = (s + 8.0) / (PI + 8.0);
    return pow(max(dot(reflect(normal, viewDir), lightDir), 0.0), s) * nrm;
}

/*
*use height to simulate sky color
*/
vec3 getSkyColor(vec3 e)
{
    e.z = max(e.z, 0.1);
    return vec3(pow(1.0 - e.z,2.0), 1.0 - e.z, 0.6 + (1.0 - e.z)*0.4);
}

/*note:
viewDir from viewPos to originPos
*/
vec3 getSeaColor(vec3 pos,vec3 normal, vec3 lightDir, vec3 viewDir)
{
    //fresnel define the percentage of energy that will reflect
    //F0 means the max percentage when theta is 90
    float fresnel = 1.0 - max(dot(normal, -viewDir), 0.0);
    fresnel = pow(fresnel, 3.0) * 0.5;

    vec3 reflected = getSkyColor(reflect(viewDir, normal));
    vec3 refracted = seaBase + diffuse(normal, lightDir, 80.0) * seaWaterColor * 0.2;

    //mix reflected and refracted with fresnel
    vec3 color = mix(refracted, reflected, fresnel);

    //add specular
    color += vec3(specular(normal, lightDir, viewDir,1400.0));
    return color;
}

void main(void)
{
    lightDirec = normalize(lightDirec);
    vec3 viewDirec = normalize(originPos - viewPos);

    fColor = vec4(mix(getSkyColor(viewDirec),
                      getSeaColor(originPos, originNormal, lightDirec, viewDirec),
                      pow(smoothstep(0.0, -0.02, viewDirec.z), 0.2)),
                  1.0);
}
