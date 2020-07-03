#ifndef OGLWINDOW_H
#define OGLWINDOW_H

#include<QOpenGLWidget>
#include<QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <vector>
#include <memory>
#include <unordered_map>

class oglWindow: public QOpenGLWidget,
        protected QOpenGLFunctions
{
    Q_OBJECT

public:
    oglWindow(QWidget * parent = nullptr);
    void setNormalVis(int state);
    void setLineVis(int state);
    void setStop(int state);
    void setWaves(int id);

    void updateParas(float A,float theta,float omega,float phi,int ind);
    void updateWavesNumber(int number);
    void setWavesNumber(int number);
    std::vector<float> &getParas();
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;


private:
    std::string shaderName = "sinWave";
    std::string debugShaderName = "sinWaveNormalVis";
    bool normalVis = false;
    bool lineVis = false;
    bool stop = false;

    void initData();
    void addShaderProgram(std::string shaderProgramName, std::string vertPath, std::string fragPath, std::string geoPath = "");
    void createBuffer();

    void printContextInformation();

    //new version
    std::unordered_map<std::string, std::unique_ptr<QOpenGLShaderProgram>> shaderPrograms;
    std::unique_ptr<QOpenGLBuffer> VBO;
    std::unique_ptr<QOpenGLBuffer> EBO;
    std::unique_ptr<QOpenGLVertexArrayObject> VAO;

    std::vector<unsigned int> indexes;
    std::vector<float> datas;
    std::vector<float> waveParas;
    unsigned int dataSize;
    float maxWaves;

    QVector3D eye{0,-3,2};
    QVector3D center{0,0,0};
    QVector3D up{0,1,0};
    QMatrix4x4 model,view,projection;
};

#endif // OGLWINDOW_H
