#include "oglwindow.h"
#include <QDebug>
#include "vertex.h"
#include <QDateTime>

void oglWindow::initializeGL()
{
    initializeOpenGLFunctions();
    printContextInformation();

    glClearColor(0.0f, 0.3f, 0.0f, 1.0f);

    initData();
    {
        //create shader program
        shaderProgram = new QOpenGLShaderProgram();
        shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertPath.toStdString().c_str());
        shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragPath.toStdString().c_str());
        if(!geoPath.isEmpty())
        {
            shaderProgram->addShaderFromSourceFile(QOpenGLShader::Geometry, geoPath.toStdString().c_str());
        }
        shaderProgram->link();
        shaderProgram->bind();

        //normla debug shader
        shaderProgram_debug = new QOpenGLShaderProgram();
        shaderProgram_debug->addShaderFromSourceFile(QOpenGLShader::Vertex, vertPath_debug.toStdString().c_str());
        shaderProgram_debug->addShaderFromSourceFile(QOpenGLShader::Fragment,fragPath_debug.toStdString().c_str());
        shaderProgram_debug->addShaderFromSourceFile(QOpenGLShader::Geometry,geoPath_debug.toStdString().c_str());
        shaderProgram_debug->bind();
        shaderProgram_debug->link();

        //create buffer
        m_vbo_ptr.reset(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer));
        m_vbo_ptr->create();
        m_vbo_ptr->bind();
        m_vbo_ptr->setUsagePattern(QOpenGLBuffer::StaticDraw);
        m_vbo_ptr->allocate(&datas[0], sizeof(float) * datas.size());

        m_ebo_ptr.reset(new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer));
        m_ebo_ptr->create();
        m_ebo_ptr->bind();
        m_ebo_ptr->setUsagePattern(QOpenGLBuffer::StaticDraw);
        m_ebo_ptr->allocate(&indexes[0], sizeof(unsigned int) * indexes.size());

        //create vertex array object
        m_vao.create();
        m_vao.bind();

        qDebug()<<"datas.size: "<<datas.size()<<endl;
        qDebug()<<"indexes.size: "<<indexes.size()<<endl;

        shaderProgram->bind();
        shaderProgram->enableAttributeArray(0);
        shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 3);
        shaderProgram->setUniformValue(shaderProgram->uniformLocation("model"), model);
        shaderProgram->setUniformValue(shaderProgram->uniformLocation("view"), view);
        shaderProgram->setUniformValue(shaderProgram->uniformLocation("projection"), projection);

        shaderProgram_debug->bind();
        shaderProgram_debug->enableAttributeArray(0);
        shaderProgram_debug->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 3);
        shaderProgram_debug->setUniformValue(shaderProgram_debug->uniformLocation("model"), model);
        shaderProgram_debug->setUniformValue(shaderProgram_debug->uniformLocation("view"), view);
        shaderProgram_debug->setUniformValue(shaderProgram_debug->uniformLocation("projection"), projection);

        m_vao.release();
        m_vbo_ptr.release();
        shaderProgram->release();
        shaderProgram_debug->release();
    }
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

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    shaderProgram->bind();
    shaderProgram->setUniformValue(shaderProgram->uniformLocation("time"),time);
    {
        m_vao.bind();
        glDrawElements(GL_TRIANGLES,indexes.size(),GL_UNSIGNED_INT,&indexes[0]);
        m_vao.release();
    }
    shaderProgram->release();

//    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
//    shaderProgram_debug->bind();
//    shaderProgram_debug->setUniformValue(shaderProgram_debug->uniformLocation("time"),time);
//    {
//        m_vao.bind();
//        glDrawElements(GL_TRIANGLES,indexes.size(),GL_UNSIGNED_INT,&indexes[0]);
//        m_vao.release();
//    }
//    shaderProgram_debug->release();

    QMetaObject::invokeMethod(this,"update",Qt::QueuedConnection);
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

    vertPath = "/home/zdxiao/Desktop/water_ogl/shaders/gerstnerwave.vert";
    fragPath = "/home/zdxiao/Desktop/water_ogl/shaders/gerstnerwave.frag";

    vertPath_debug = "/home/zdxiao/Desktop/water_ogl/shaders/normal_gerstner_visualization.vert";
    fragPath_debug = "/home/zdxiao/Desktop/water_ogl/shaders/normal_gerstner_visualization.frag";
    geoPath_debug  = "/home/zdxiao/Desktop/water_ogl/shaders/normal_gerstner_visualization.geom";

    //config model and projection matrix
    projection.perspective(45.0f,static_cast<float>(this->width())/static_cast<float>(this->height()),0.1f,100.0f);
    view.lookAt(eye,center,up);
    qDebug()<<"model: "<<model<<endl;
    qDebug()<<"view: "<<view<<endl;
    qDebug()<<"projection: "<<projection<<endl;
}

oglWindow::oglWindow()
{

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