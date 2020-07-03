#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "oglwindow.h"
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow: public QMainWindow
{
     Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();
    Ui::MainWindow * ui;

public slots:
    void setNormalVisStatus(int state);
    void setLineModeStatus(int state);
    void setStopModeStatus(int state);

    void selectWave();
    void updateMaxWaveNumber();
    void updateParam1();
    void updateParam2();
    void updateParam3();
    void updateParam4();
    void updateParam5();
    void updateParam6();
private:
    void connectSlots();
    void passInitData(std::vector<float> &paras,int waveNumber);
};

#endif // MAINWINDOW_H
