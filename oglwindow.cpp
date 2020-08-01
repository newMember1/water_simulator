#include "oglwindow.h"
#include <QDebug>
#include "vertex.h"
#include <QDateTime>
#include <QCoreApplication>
#include <math.h>
#include <QOpenGLExtraFunctions>

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
}

void oglWindow::setShaderAttribute(std::string vaoName)
{
    for(auto it = shaderPrograms.begin();it != shaderPrograms.end(); ++it)
    {
        if(vaoName == "VAO" && it->first != "oceanCpu" && it->first != "oceanCpuNormalVis")
        {
            it->second->bind();
            it->second->enableAttributeArray(0);
            it->second->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 3);
        }
        else if(vaoName == "oceanCpuVAO" && (it->first == "oceanCpu" || it->first == "oceanCpuNormalVis"))
        {
            it->second->bind();
            it->second->enableAttributeArray(0);
            it->second->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 3);
            it->second->enableAttributeArray(1);
            it->second->setAttributeBuffer(1, GL_FLOAT, datas.size() * sizeof(float), 3, sizeof(float) * 3);
        }
    }
}

void oglWindow::setShaderUniforms()
{
    for(auto it = shaderPrograms.begin(); it != shaderPrograms.end(); ++it)
    {
        it->second->bind();
        it->second->setUniformValue(it->second->uniformLocation("model"), model);
        it->second->setUniformValue(it->second->uniformLocation("view"), view);
        it->second->setUniformValue(it->second->uniformLocation("projection"), projection);
        it->second->setUniformValue(it->second->uniformLocation("maxWaves"),maxWaves);

        if(it->first != "oceanCpu" && it->first != "oceanCpuNormalVis")
        {
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

                it->second->setUniformValue(it->second->uniformLocation((s+A).c_str()),waveParas[i * 4]);
                it->second->setUniformValue(it->second->uniformLocation((s+direc).c_str()),dir);
                it->second->setUniformValue(it->second->uniformLocation((s+omega).c_str()),waveParas[i * 4 + 2]);
                it->second->setUniformValue(it->second->uniformLocation((s+phi).c_str()),waveParas[i * 4 + 3]);
            }
        }
    }
}

void oglWindow::createBuffer()
{
    //common buffer setting
    VAO.reset(new QOpenGLVertexArrayObject());
    VAO->create();
    VAO->bind();

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

    setShaderAttribute("VAO");
    EBO->release();
    VBO->release();
    VAO->release();

    //ocean cpu buffer setting
    oceanCpuVAO.reset(new QOpenGLVertexArrayObject());
    oceanCpuVAO->create();
    oceanCpuVAO->bind();

    oceanCpuVBO.reset(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer));
    oceanCpuVBO->create();
    oceanCpuVBO->bind();
    oceanCpuVBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    oceanCpuVBO->allocate(datas.size() * sizeof(float) * 2);

    oceanCpuEBO.reset(new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer));
    oceanCpuEBO->create();
    oceanCpuEBO->bind();
    oceanCpuEBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    oceanCpuEBO->allocate(&indexes[0], sizeof(unsigned int) * indexes.size());

    setShaderAttribute("oceanCpuVAO");
    oceanCpuEBO->release();
    oceanCpuVBO->release();
    oceanCpuVAO->release();
}

void oglWindow::initializeGL()
{
    initializeOpenGLFunctions();
    printContextInformation();

    glEnable(GL_DEBUG_OUTPUT);

    initData();
    //sinwave
    addShaderProgram("sinWave",":/shaderRes/shaders/sinwave.vert",":/shaderRes/shaders/waterColor.frag");
    addShaderProgram("sinWaveNormalVis",":/shaderRes/shaders/normal_sinwave_visualization.vert",":/shaderRes/shaders/normal_visualization.frag",":/shaderRes/shaders/normal_visualization.geom");

    //gerstnerwave
    addShaderProgram("gerstnerWave",":/shaderRes/shaders/gerstnerwave.vert",":/shaderRes/shaders/waterColor.frag");
    addShaderProgram("gerstnerWaveNormalVis",":/shaderRes/shaders/normal_gerstner_visualization.vert",":/shaderRes/shaders/normal_visualization.frag",":/shaderRes/shaders/normal_visualization.geom");

    //oceanCpu
    addShaderProgram("oceanCpu",":/shaderRes/shaders/ocean_cpu.vert",":/shaderRes/shaders/waterColor.frag");
    addShaderProgram("oceanCpuNormalVis",":/shaderRes/shaders/normal_ocean_cpu_visualization.vert",":/shaderRes/shaders/normal_visualization.frag",":/shaderRes/shaders/normal_visualization.geom");

    createBuffer();
    setShaderUniforms();

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

    //enable line mode
    if(lineVis)
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }

    //oceanCpu
    if(refreshGrid)
    {
        //use glbuffersubdata
        oceanCpuVAO->bind();
        shaderPrograms[shaderName]->bind();

        oceanGenerater->HDNs(time);
        auto verts = oceanGenerater->getNewVertexs();
        auto ns = oceanGenerater->getNormals();

        oceanCpuVBO->bind();
        oceanCpuVBO->write(0, &verts[0], verts.size() * sizeof(float));
        oceanCpuVBO->write(verts.size() * sizeof(float), &ns[0], ns.size() * sizeof(float));

        glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_INT, &indexes[0]);

        oceanCpuVBO->release();
        shaderPrograms[shaderName]->release();
        oceanCpuVAO->release();
    }
    else
    {
        //normally draw water mesh
        VAO->bind();
        shaderPrograms[shaderName]->bind();

        shaderPrograms[shaderName]->setUniformValue(shaderPrograms[shaderName]->uniformLocation("time"),time);
        glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_INT, &indexes[0]);

        shaderPrograms[shaderName]->release();
        VAO->release();
    }

    //draw normal
    if(normalVis)
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        if(refreshGrid)
        {
            shaderPrograms[debugShaderName]->bind();
            oceanCpuVAO->bind();
            oceanCpuVBO->bind();
            auto verts = oceanGenerater->getNewVertexs();
            auto ns = oceanGenerater->getNormals();
            oceanCpuVBO->write(0, &verts[0], verts.size() * sizeof(float));
            oceanCpuVBO->write(verts.size() * sizeof(float), &ns[0], ns.size() * sizeof(float));
            glDrawElements(GL_TRIANGLES,indexes.size(),GL_UNSIGNED_INT,&indexes[0]);
            oceanCpuVBO->release();
            oceanCpuVAO->release();
        }
        else
        {
            shaderPrograms[debugShaderName]->bind();
            shaderPrograms[debugShaderName]->setUniformValue(shaderPrograms[debugShaderName]->uniformLocation("time"),time);
            VAO->bind();
            glDrawElements(GL_TRIANGLES,indexes.size(),GL_UNSIGNED_INT,&indexes[0]);
            VAO->release();
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
    this->gridN = 50;
    this->gridLx = 3.0;
    this->gridLy = 3.0;
    float deltaX = this->gridLx / this->gridN;
    float deltaY = this->gridLy / this->gridN;

    for(int i=0;i<gridN;++i)
        for(int j=0;j<gridN;++j)
        {
            float x,y,z;
            x = -gridLx / 2 + deltaX * j;
            y = -gridLy / 2 + deltaY * i;

            z = 0;
            datas.push_back(x);
            datas.push_back(y);
            datas.push_back(z);
        }

    for(int i=0;i<gridN-1;++i)
        for(int j=0;j<gridN-1;++j)
        {
            unsigned int ia = j + i * gridN;
            unsigned int ib = ia + 1;
            unsigned int ic = ia + gridN;

            indexes.push_back(ia);
            indexes.push_back(ib);
            indexes.push_back(ic);

            ia = ib;
            ib = ia + gridN;
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
    if(id == 0)
    {
        refreshGrid = false;
        shaderName = "sinWave";
        debugShaderName = "sinWaveNormalVis";
    }
    else if(id == 1)
    {
        refreshGrid = false;
        shaderName = "gerstnerWave";
        debugShaderName = "gerstnerWaveNormalVis";
    }
    else if(id == 2)
    {
        refreshGrid = true;
        if(!oceanGenerater)
            initOceanVertexes();
        shaderName = "oceanCpu";
        debugShaderName = "oceanCpuNormalVis";
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

void oglWindow::initOceanVertexes()
{
    this->oceanGenerater.reset(new ocean_cpu(gridLx, gridLy, gridN, gridN, gridA, gridDw, gridVw, datas));
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
