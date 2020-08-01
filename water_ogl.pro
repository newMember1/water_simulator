QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += console debug
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ocean_cpu.cpp \
    oglwindow.cpp

HEADERS += \
    mainwindow.h \
    ocean_cpu.h \
    oglwindow.h \
    vertex.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    shaders/gerstnerwave.vert \
    shaders/normal_gerstner_visualization.geom \
    shaders/normal_gerstner_visualization.vert \
    shaders/normal_ocean_cpu_visualization.vert \
    shaders/normal_sinwave_visualization.frag \
    shaders/normal_sinwave_visualization.geom \
    shaders/normal_sinwave_visualization.vert \
    shaders/normal_visualization.frag \
    shaders/normal_visualization.geom \
    shaders/ocean_cpu.frag \
    shaders/ocean_cpu.vert \
    shaders/phillipsspectrum_old.vert \
    shaders/sinwave.vert \
    shaders/sinwave.vert \
    shaders/waterColor.frag

RESOURCES += \
    shaders.qrc
