#include "oglwindow.h"
#include <QDebug>
#include "vertex.h"
#include <QDateTime>
#include <QCoreApplication>
#include <math.h>

oglWindow::oglWindow(QWidget * parent):QOpenGLWidget(parent)
{

}

void oglWindow::addShaderProgram(std::string name, std::string vertPath, std::string fragPath, std::string geoPath)
{
    shaderPrograms[name].reset(new QOpenGLShaderProgram());
    shaderPrograms[name]->addShaderFromSourceFile(QOpenGLShader::Vertex,vertPath.c_str());
    shaderPrograms[name]->addShaderFromSourceFile(QOpenGLShader::Fragment,fragPath.c_str());
    if(!geoPath.empty())
    {
        shaderPrograms[name]->addShaderFromSourceFile(QOpenGLShader::Geometry, geoPath.c_str());
    }

    shaderPrograms[name]->link();
    shaderPrograms[name]->bind();
    shaderPrograms[name]->enableAttributeArray(0);
    shaderPrograms[name]->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 3);
    shaderPrograms[name]->setUniformValue(shaderPrograms[name]->uniformLocation("model"), model);
    shaderPrograms[name]->setUniformValue(shaderPrograms[name]->uniformLocation("view"), view);
    shaderPrograms[name]->setUniformValue(shaderPrograms[name]->uniformLocation("projection"), projection);
    shaderPrograms[name]->setUniformValue(shaderPrograms[name]->uniformLocation("maxWaves"),maxWaves);

    std::string A = ".A";
    std::string omega = ".omega";
    std::string phi = ".phi";
    std::string direc = ".Dxy";
    QVector2D dir;
    for(int i=0;i<6;++i)
    {
        dir[0] = cos(waveParas[i * 4 + 1]);
        dir[1] = sin(waveParas[i * 4 + 1]);
        std::string s = "waves[";
        s+=std::to_string(i) + "]";

        shaderPrograms[name]->setUniformValue(shaderPrograms[name]->uniformLocation((s+A).c_str()),waveParas[i * 4]);
        shaderPrograms[name]->setUniformValue(shaderPrograms[name]->uniformLocation((s+direc).c_str()),dir);
        shaderPrograms[name]->setUniformValue(shaderPrograms[name]->uniformLocation((s+omega).c_str()),waveParas[i * 4 + 2]);
        shaderPrograms[name]->setUniformValue(shaderPrograms[name]->uniformLocation((s+phi).c_str()),waveParas[i * 4 + 3]);
    }
}

void oglWindow::createBuffer()
{
    VBO.reset(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer));
    VBO->create();
    VBO->bind();
    VBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    VBO->allocate(&datas[0], sizeof(float) * datas.size());

    EBO.reset(new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer));
    EBO->create();
    EBO->bind();
    EBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    EBO->allocate(&indexes[0], sizeof(unsigned int) * indexes.size());

    VAO.reset(new QOpenGLVertexArrayObject());
    VAO->create();
    VAO->bind();
}

void oglWindow::initializeGL()
{
    initializeOpenGLFunctions();
    printContextInformation();

    initData();
    createBuffer();
    addShaderProgram("sinWave",":/shaderRes/shaders/sinwave.vert",":/shaderRes/shaders/waterColor.frag");
    addShaderProgram("sinWaveNormalVis",":/shaderRes/shaders/normal_sinwave_visualization.vert",":/shaderRes/shaders/normal_visualization.frag",":/shaderRes/shaders/normal_visualization.geom");

    addShaderProgram("gerstnerWave",":/shaderRes/shaders/gerstnerwave.vert",":/shaderRes/shaders/waterColor.frag");
    addShaderProgram("gerstnerWaveNormalVis",":/shaderRes/shaders/normal_gerstner_visualization.vert",":/shaderRes/shaders/normal_visualization.frag",":/shaderRes/shaders/normal_visualization.geom");

    glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void oglWindow::resizeGL(int w, int h)
{
    (void)w;
    (void)h;
}

void oglWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    //render
    QDateTime cTime = QDateTime::currentDateTime();
    float time = static_cast<float>(QDateTime::currentMSecsSinceEpoch() /200 % 500);

    if(lineVis)
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
    shaderPrograms[shaderName]->bind();
    shaderPrograms[shaderName]->setUniformValue(shaderPrograms[shaderName]->uniformLocation("time"),time);
    {
        VAO->bind();
        glDrawElements(GL_TRIANGLES,indexes.size(),GL_UNSIGNED_INT,&indexes[0]);
    }

    if(normalVis)
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        shaderPrograms[debugShaderName]->bind();
        shaderPrograms[debugShaderName]->setUniformValue(shaderPrograms[debugShaderName]->uniformLocation("time"),time);
        {
            VAO->bind();
            glDrawElements(GL_TRIANGLES,indexes.size(),GL_UNSIGNED_INT,&indexes[0]);
        }
    }

    if(!stop)
    {
        QMetaObject::invokeMethod(this,"update",Qt::QueuedConnection);
    }
}

void oglWindow::initData()
{
    //generate a n*n grid
    int n = 50;
    float delta = 3.0 / (n - 1);

    for(int i=0;i<n;++i)
        for(int j=0;j<n;++j)
        {
            float x,y,z;
            x = -1.5 + delta * j;
            y = -1.5 + delta * i;
            z = 0;
            datas.push_back(x);
            datas.push_back(y);
            datas.push_back(z);
        }

    for(int i=0;i<n-1;++i)
        for(int j=0;j<n-1;++j)
        {
            unsigned int ia = j + i * n;
            unsigned int ib = ia + 1;
            unsigned int ic = ia + n;

            indexes.push_back(ia);
            indexes.push_back(ib);
            indexes.push_back(ic);

            ia = ib;
            ib = ia + n;
            ic = ib -1;
            indexes.push_back(ia);
            indexes.push_back(ib);
            indexes.push_back(ic);
        }

    //config model and projection matrix
    projection.perspective(45.0f,static_cast<float>(this->width())/static_cast<float>(this->height()),0.1f,100.0f);
    view.lookAt(eye,center,up);
}

void oglWindow::setLineVis(int state)
{
    if(state == Qt::Checked)
        lineVis = true;
    else
        lineVis = false;
}

void oglWindow::setNormalVis(int state)
{
    if(state == Qt::Checked)
        normalVis = true;
    else
        normalVis = false;
}

void oglWindow::setStop(int state)
{
    if(state == Qt::Checked)
        stop = true;
    else
    {
        stop = false;
        update();
    }
}

void oglWindow::setWaves(int id)
{
    qDebug()<<"id is: "<<id<<endl;
    if(id == 0)
    {
        shaderName = "sinWave";
        debugShaderName = "sinWaveNormalVis";
    }
    else if(id == 1)
    {
        shaderName ="gerstnerWave";
        debugShaderName = "gerstnerWaveNormalVis";
    }
}

void oglWindow::updateParas(float A, float theta, float omega, float phi, int ind)
{
    waveParas[4 * ind] = A;
    waveParas[4 * ind + 1] = theta;
    waveParas[4 * ind + 2] = omega;
    waveParas[4 * ind + 3] = phi;
    float dX = cos(theta);
    float dY = sin(theta);

    std::string sA = ".A";
    std::string sOmega = ".omega";
    std::string sPhi = ".phi";
    std::string sDirec = ".Dxy";
    std::string sS = "waves[";
    sS+=std::to_string(ind) + "]";
    QVector2D dir;
    dir[0] = dX;
    dir[1] = dY;
    shaderPrograms[shaderName]->bind();
    shaderPrograms[shaderName]->setUniformValue(shaderPrograms[shaderName]->uniformLocation((sS+sA).c_str()),A);
    shaderPrograms[shaderName]->setUniformValue(shaderPrograms[shaderName]->uniformLocation((sS+sDirec).c_str()),dir);
    shaderPrograms[shaderName]->setUniformValue(shaderPrograms[shaderName]->uniformLocation((sS+sOmega).c_str()),omega);
    shaderPrograms[shaderName]->setUniformValue(shaderPrograms[shaderName]->uniformLocation((sS+sPhi).c_str()),phi);

    shaderPrograms[debugShaderName]->bind();
    shaderPrograms[debugShaderName]->setUniformValue(shaderPrograms[debugShaderName]->uniformLocation((sS+sA).c_str()),A);
    shaderPrograms[debugShaderName]->setUniformValue(shaderPrograms[debugShaderName]->uniformLocation((sS+sDirec).c_str()),dir);
    shaderPrograms[debugShaderName]->setUniformValue(shaderPrograms[debugShaderName]->uniformLocation((sS+sOmega).c_str()),omega);
    shaderPrograms[debugShaderName]->setUniformValue(shaderPrograms[debugShaderName]->uniformLocation((sS+sPhi).c_str()),phi);
}

void oglWindow::updateWavesNumber(int number)
{
    setWavesNumber(number);
    shaderPrograms[shaderName]->bind();
    shaderPrograms[shaderName]->setUniformValue(shaderPrograms[shaderName]->uniformLocation("maxWaves"),maxWaves);
    shaderPrograms[debugShaderName]->bind();
    shaderPrograms[debugShaderName]->setUniformValue(shaderPrograms[shaderName]->uniformLocation("maxWaves"),maxWaves);
}

void oglWindow::setWavesNumber(int number)
{
    maxWaves = number;
}

std::vector<float> &oglWindow::getParas()
{
    return this->waveParas;
}

void oglWindow::printContextInformation()
{
    QString glType;
    QString glVersion;
    QString glProfile;

    glType=(context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
    glVersion=reinterpret_cast<const char *>(glGetString(GL_VERSION));

#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
    switch(format().profile())
    {
        CASE(NoProfile);
        CASE(CoreProfile);
        CASE(CompatibilityProfile);
    }
#undef CASE
    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}
