#include <QGuiApplication>
#include "oglwindow.h"

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    //set OpenGL Version information
    //Note: This format must be se before show() is called.
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,3);

    oglWindow w;
    w.setFormat(format);
    w.resize(QSize(800,600));
    w.show();
    return a.exec();
}
