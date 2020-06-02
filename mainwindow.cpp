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

    connectSlots();
}

void MainWindow::connectSlots()
{
    auto UI = this->ui;
    connect(UI->lineMode,SIGNAL(stateChanged(int)),this,SLOT(setLineModeStatus(int)));
    connect(UI->NormalVis,SIGNAL(stateChanged(int)),this,SLOT(setNormalVisStatus(int)));

}

void MainWindow::setLineModeStatus(int state)
{
    this->ui->openGLWidget->setLineVis(state);
}

void MainWindow::setNormalVisStatus(int state)
{
    this->ui->openGLWidget->setNormalVis(state);
}

MainWindow::~MainWindow()
{
    delete ui;
}
