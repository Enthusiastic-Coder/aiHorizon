#include <QGuiApplication>

#include "openglwindow.h"
#include <QSize>
#include <QScreen>

int main(int argc, char *argv[])
{
#ifdef WIN32
    SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED);
#endif
    QGuiApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    QGuiApplication a(argc, argv);

    OpenGLWindow window;
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    fmt.setStencilBufferSize(8);
    //fmt.setMajorVersion(2);
    //fmt.setMinorVersion(0);
//   fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
//    fmt.setRenderableType(QSurfaceFormat::OpenGLES);
    window.setFormat(fmt);

#ifndef ANDROID
    QSize size = QGuiApplication::screens()[0]->availableSize();
    QSize newSize(size.width()*0.8, size.height()*0.8);
    window.resize(newSize);
#endif
    window.show();

    return a.exec();
}
