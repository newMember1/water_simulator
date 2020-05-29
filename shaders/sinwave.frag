#version 330 core

out vec4 fColor;
in vec3 originNormal;
in vec3 originPos;

void main(void)
{
    vec3 lightDirec = vec3(-1.0/14,-2.0/14,-3.0/14);
    vec3 lightColor = vec3(0.0/255.0,55.0/255.0,120.0/255.0);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    float diff = max(dot(-lightDirec, originNormal), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength= 0.5;
    vec3 viewDir = normalize(vec3(0,-3,2) - originPos);
    vec3 reflectDir = reflect(lightDirec,originNormal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),2.0);
    vec3 specular = specularStrength * spec *lightColor;

    fColor = vec4(ambient + diffuse + specular,1.0);

//    if(originNormal.y<0&&originNormal.x<0)
//        fColor = vec4(1.0,0.0,0.0,1.0);
//    else if(originNormal.y<0)
//        fColor = vec4(0.0,1.0,0.0,1.0);
//    else if(originNormal.x<0)
//        fColor = vec4(0.0,0.0,1.0,1.0);
//    else
//        fColor = vec4(0.1,0.1,0.1,1.0);
}
