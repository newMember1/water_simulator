#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,3);
    ui->openGLWidget->setFormat(format);
    ui->openGLWidget->setVisible(true);

    ui->radioButtonSinwave->setChecked(true);
    connectSlots();
    passInitData(ui->openGLWidget->getParas(),ui->maxWavesNumber->value());
}

void MainWindow::connectSlots()
{
    auto UI = this->ui;
    connect(UI->lineMode,SIGNAL(stateChanged(int)),this,SLOT(setLineModeStatus(int)));
    connect(UI->NormalVis,SIGNAL(stateChanged(int)),this,SLOT(setNormalVisStatus(int)));
    connect(UI->stopMode,SIGNAL(stateChanged(int)),this,SLOT(setStopModeStatus(int)));
    connect(UI->radioButtonSinwave,SIGNAL(clicked(bool)),this,SLOT(selectWave()));
    connect(UI->radioButtonGerstnerWave,SIGNAL(clicked(bool)),this,SLOT(selectWave()));
    connect(UI->maxWavesNumber,SIGNAL(valueChanged(int)),this,SLOT(updateMaxWaveNumber()));

    //set 6 wave paras
    connect(UI->paraA1,SIGNAL(valueChanged(double)),this,SLOT(updateParam1()));
    connect(UI->paraTheta1,SIGNAL(valueChanged(double)),this,SLOT(updateParam1()));
    connect(UI->paraOmega1,SIGNAL(valueChanged(double)),this,SLOT(updateParam1()));
    connect(UI->paraPhi1,SIGNAL(valueChanged(double)),this,SLOT(updateParam1()));

    connect(UI->paraA2,SIGNAL(valueChanged(double)),this,SLOT(updateParam2()));
    connect(UI->paraTheta2,SIGNAL(valueChanged(double)),this,SLOT(updateParam2()));
    connect(UI->paraOmega2,SIGNAL(valueChanged(double)),this,SLOT(updateParam2()));
    connect(UI->paraPhi2,SIGNAL(valueChanged(double)),this,SLOT(updateParam2()));

    connect(UI->paraA3,SIGNAL(valueChanged(double)),this,SLOT(updateParam3()));
    connect(UI->paraTheta3,SIGNAL(valueChanged(double)),this,SLOT(updateParam3()));
    connect(UI->paraOmega3,SIGNAL(valueChanged(double)),this,SLOT(updateParam3()));
    connect(UI->paraPhi3,SIGNAL(valueChanged(double)),this,SLOT(updateParam3()));

    connect(UI->paraA4,SIGNAL(valueChanged(double)),this,SLOT(updateParam4()));
    connect(UI->paraTheta4,SIGNAL(valueChanged(double)),this,SLOT(updateParam4()));
    connect(UI->paraOmega4,SIGNAL(valueChanged(double)),this,SLOT(updateParam4()));
    connect(UI->paraPhi4,SIGNAL(valueChanged(double)),this,SLOT(updateParam4()));

    connect(UI->paraA5,SIGNAL(valueChanged(double)),this,SLOT(updateParam5()));
    connect(UI->paraTheta5,SIGNAL(valueChanged(double)),this,SLOT(updateParam5()));
    connect(UI->paraOmega5,SIGNAL(valueChanged(double)),this,SLOT(updateParam5()));
    connect(UI->paraPhi5,SIGNAL(valueChanged(double)),this,SLOT(updateParam5()));

    connect(UI->paraA6,SIGNAL(valueChanged(double)),this,SLOT(updateParam6()));
    connect(UI->paraTheta6,SIGNAL(valueChanged(double)),this,SLOT(updateParam6()));
    connect(UI->paraOmega6,SIGNAL(valueChanged(double)),this,SLOT(updateParam6()));
    connect(UI->paraPhi6,SIGNAL(valueChanged(double)),this,SLOT(updateParam6()));
}

void MainWindow::setLineModeStatus(int state)
{
    this->ui->openGLWidget->setLineVis(state);
}

void MainWindow::setNormalVisStatus(int state)
{
    this->ui->openGLWidget->setNormalVis(state);
}

void MainWindow::setStopModeStatus(int state)
{
    this->ui->openGLWidget->setStop(state);
}

void MainWindow::selectWave()
{
    if(this->ui->radioButtonSinwave->isChecked())
        this->ui->openGLWidget->setWaves(0);
    else if(this->ui->radioButtonGerstnerWave->isChecked())
        this->ui->openGLWidget->setWaves(1);
    else
        this->ui->openGLWidget->setWaves(2);
}

void MainWindow::updateMaxWaveNumber()
{
    this->ui->openGLWidget->updateWavesNumber(this->ui->maxWavesNumber->value());
}

void MainWindow::passInitData(std::vector<float> &paras,int waveNumber)
{
    paras.push_back(this->ui->paraA1->value());
    paras.push_back(this->ui->paraTheta1->value());
    paras.push_back(this->ui->paraOmega1->value());
    paras.push_back(this->ui->paraPhi1->value());

    paras.push_back(this->ui->paraA2->value());
    paras.push_back(this->ui->paraTheta2->value());
    paras.push_back(this->ui->paraOmega2->value());
    paras.push_back(this->ui->paraPhi2->value());

    paras.push_back(this->ui->paraA3->value());
    paras.push_back(this->ui->paraTheta3->value());
    paras.push_back(this->ui->paraOmega3->value());
    paras.push_back(this->ui->paraPhi3->value());

    paras.push_back(this->ui->paraA4->value());
    paras.push_back(this->ui->paraTheta4->value());
    paras.push_back(this->ui->paraOmega4->value());
    paras.push_back(this->ui->paraPhi4->value());

    this->ui->openGLWidget->setWavesNumber(waveNumber);
}

void MainWindow::updateParam1()
{
    float A = this->ui->paraA1->value();
    float theta = this->ui->paraTheta1->value();
    float omega = this->ui->paraOmega1->value();
    float phi = this->ui->paraPhi1->value();

    this->ui->openGLWidget->updateParas(A,theta,omega,phi,0);
}

void MainWindow::updateParam2()
{
    float A = this->ui->paraA2->value();
    float theta = this->ui->paraTheta2->value();
    float omega = this->ui->paraOmega2->value();
    float phi = this->ui->paraPhi2->value();

    this->ui->openGLWidget->updateParas(A,theta,omega,phi,1);
}

void MainWindow::updateParam3()
{
    float A = this->ui->paraA3->value();
    float theta = this->ui->paraTheta3->value();
    float omega = this->ui->paraOmega3->value();
    float phi = this->ui->paraPhi3->value();

    this->ui->openGLWidget->updateParas(A,theta,omega,phi,2);
}

void MainWindow::updateParam4()
{
    float A = this->ui->paraA4->value();
    float theta = this->ui->paraTheta4->value();
    float omega = this->ui->paraOmega4->value();
    float phi = this->ui->paraPhi4->value();

    this->ui->openGLWidget->updateParas(A,theta,omega,phi,3);
}

void MainWindow::updateParam5()
{
    float A = this->ui->paraA5->value();
    float theta = this->ui->paraTheta5->value();
    float omega = this->ui->paraOmega5->value();
    float phi = this->ui->paraPhi5->value();

    this->ui->openGLWidget->updateParas(A,theta,omega,phi,4);
}

void MainWindow::updateParam6()
{
    float A = this->ui->paraA6->value();
    float theta = this->ui->paraTheta6->value();
    float omega = this->ui->paraOmega6->value();
    float phi = this->ui->paraPhi6->value();

    this->ui->openGLWidget->updateParas(A,theta,omega,phi,5);
}

MainWindow::~MainWindow()
{
    delete ui;
}
