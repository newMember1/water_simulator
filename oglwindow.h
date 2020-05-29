#ifndef OGLWINDOW_H
#define OGLWINDOW_H

#include<QOpenGLWindow>
#include<QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <vector>
#include <memory>

class oglWindow: public QOpenGLWindow,
        protected QOpenGLFunctions
{
    Q_OBJECT

public:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void initData();
    oglWindow();

private:
    void printContextInformation();

    QString vertPath;
    QString fragPath;
    QString geoPath;

    QString vertPath_debug;
    QString fragPath_debug;
    QString geoPath_debug;

    QString vertPath_simple;
    QString fragPath_simple;

    QOpenGLVertexArrayObject m_vao;
    QOpenGLShaderProgram *shaderProgram;
    QOpenGLShaderProgram *shaderProgram_debug;

    std::unique_ptr<QOpenGLBuffer> m_vbo_ptr = nullptr;
    std::unique_ptr<QOpenGLBuffer> m_ebo_ptr = nullptr;

    std::vector<unsigned int> indexes;
    std::vector<float> datas;
    unsigned int dataSize;

    QVector3D eye{0,-3,2};
    QVector3D center{0,0,0};
    QVector3D up{0,1,0};
    QMatrix4x4 model,view,projection;
};

#endif // OGLWINDOW_H
