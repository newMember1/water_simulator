#include "ocean_cpu.h"
#include<QDebug>

ocean_cpu::ocean_cpu(int lx, int ly, int nx, int ny, float a, QVector2D dw, float vw, std::vector<float> vertexs)
{
    this->Lx = lx;
    this->Ly = ly;
    this->Nx = nx;
    this->Ny = ny;
    this->A = a;
    this->Dw = dw;
    this->Vw = vw;
    this->Vertexs = vertexs;
    this->newVertexs = this->Vertexs;
    this->normals = this->Vertexs;
    //first calculate every point's frequency
    std::complex<float> htilde0;
    std::complex<float> conHtilde0;

    for(int i=0;i<Nx;++i)
    {
        float n = PI * (2 * i - Nx) / Lx;
        std::vector<QVector2D> tk;
        for(int j=0;j<Ny;++j)
        {
            float m = PI * (2 * j - Ny) / Ly;
            QVector2D k(n,m);
            ks.push_back(k);
        }
    }

    for(int i=0;i<Nx;++i)
    {
        std::vector<std::complex<float>> t;
        std::vector<std::complex<float>> conT;

        for(int j=0;j<Ny;++j)
        {
            htilde0 = hTilde0(i, j);
            conHtilde0 = std::conj(htilde0);
            t.push_back(htilde0);
            conT.push_back(conHtilde0);
        }
        htildes.push_back(t);
        conHtildes.push_back(conT);
    }
}

void ocean_cpu::HDNs(float t)
{
    //loop all vertexes
    for(int i=0;i<Nx;++i)
        for(int j=0;j<Ny;++j)
            H_D_N(t, i, j);
}

void ocean_cpu::H_D_N(float t, int xInd, int yInd)
{
    std::complex<float> H;
    QVector2D D(0, 0);
    QVector3D N(0, 0, 0);

    //kx = 2*PI*n/Lx
    //ky = 2*PI*m/Ly
    float lenK, kDotX;

    int ind = xInd * Ny + yInd;
    QVector2D xy(Vertexs[3 * ind], Vertexs[3 * ind + 1]);
    std::complex<float> eIKX;
    std::complex<float> htildeC;

    for(int i=0;i<Nx;++i)
    {
        for(int j=0;j<Ny;++j)
        {
            auto k = ks[i * Ny + j];
            lenK = k.length();
            kDotX = QVector2D::dotProduct(k, xy);
            //e^ikx
            eIKX = std::complex<float> (cos(kDotX), sin(kDotX));
            htildeC = hTilde(t, i, j) * eIKX;

            //calculate H
            H += htildeC;
            
            //calculate D
            std::complex<float> t = htildeC * std::complex<float>(0, -1);
            if(lenK > epslion)
            {
                D += QVector2D(t.real(), t.real()) / lenK * k;
            }

            //calculate N
            t = htildeC * std::complex<float>(0, 1);
            N += QVector3D(QVector2D(t.real() * k.x(), t.real() * k.y()), 0);
        }
    }

    N = QVector3D(0, 0, 1) - N;
    //origin pos
    newVertexs[3 * ind] = Vertexs[3 * ind] + D.x();
    newVertexs[3 * ind + 1] = Vertexs[3 * ind + 1] + D.y();
    newVertexs[3 * ind + 2] = H.real();

    //origin normal
    normals[3 * ind] = N.x();
    normals[3 * ind + 1] = N.y();
    normals[3 * ind + 2] = N.z();
}

std::complex<float> ocean_cpu::hTilde(float time, int n, int m)
{
    auto k = ks[n * Ny + m];
    auto htilde0 = htildes[n][m];
    auto conhtilde0 = conHtildes[n][m];

    float wk = sqrt(G * k.length());
    float cos_ = cos(wk * time);
    float sin_ = sin(wk * time);

    std::complex<float> e0(cos_, sin_);
    std::complex<float> e1(cos_, -sin_);

    return e0 * htilde0 + e1 * conhtilde0;
}

std::complex<float> ocean_cpu::hTilde0(int n, int m)
{
    //1 / sqrt(2) * (Er + i Ei)
    std::complex<float> r = gaussianRandom();
    float p = sqrt(phillips(n, m) / 2);
    r = r * p;
    return r;
}

std::vector<float> & ocean_cpu::getNewVertexs()
{
    return this->newVertexs;
}

std::vector<float> & ocean_cpu::getNormals()
{
    return this->normals;
}

std::complex<float> ocean_cpu::gaussianRandom()
{
    //generate gaussian random (x,y)
    float U = drand48();
    float V = drand48();

    float X = cos(2 * PI * U) * sqrt(-2 * log(U));
    float Y = cos(2 * PI * V) * sqrt(-2 * log(V));
    return std::complex<float> (X,Y);
}

float ocean_cpu::phillips(int n, int m)
{
    //gnerate Ph(k)
    auto k = ks[n * Ny + m];
    float lenK = k.length();
    if(lenK < 1e-6) return 0;

    float lenK4 = pow(lenK, 4);
    auto nK = k;
    auto nDw = Dw;
    nK.normalize();
    nDw.normalize();

    float kDotD = QVector2D::dotProduct(nK,nDw);
    float L = Vw * Vw / G;
    return A * exp(-1 / (lenK * lenK * L * L)) / lenK4 * kDotD * kDotD;
}
