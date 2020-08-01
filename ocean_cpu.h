#ifndef OCEAN_PU_H
#define OCEAN_CPU_H
#include<vector>
#include<QVector2D>
#include<QVector3D>
#include<complex>

const float PI = 3.1415926;
const float epslion = 1e-6;
class ocean_cpu
{
public:
    ocean_cpu(int lx, int ly, int nx, int ny, float a, QVector2D dw, float vw, std::vector<float> vertexs);

    void HDNs(float t);
    void H_D_N(float t, int xInd, int yInd);
    std::complex<float> hTilde(float time, int n, int m);
    std::complex<float> hTilde0(int n, int m);
    std::vector<float> & getNewVertexs();
    std::vector<float> & getNormals();
private:
    std::complex<float> gaussianRandom();
    float phillips(int n, int m);

    std::vector<std::vector<std::complex<float>>> htildes;
    std::vector<std::vector<std::complex<float>>> conHtildes;
    std::vector<QVector2D> ks;

    std::vector<float> newVertexs;
    std::vector<float> normals;
    std::vector<float> Vertexs;

    const float G = 9.8;
    float A;

    QVector2D Dw;
    float Vw;
    float Lx;
    float Ly;
    int Nx;
    int Ny;
};

#endif // OCEAN_CPU_H
