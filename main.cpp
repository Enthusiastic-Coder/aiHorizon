#include <QGuiApplication>

#include "openglwindow.h"
#include <QSize>
#include <QScreen>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <QDir>

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED);
#endif
//    QGuiApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
//    QGuiApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    fmt.setAlphaBufferSize(8);
    fmt.setStencilBufferSize(8);
    QSurfaceFormat::setDefaultFormat(fmt);

    QGuiApplication a(argc, argv);

    OpenGLWindow window;
#ifndef ANDROID
    QSize size = QGuiApplication::screens()[0]->availableSize();
    QSize newSize(size.width()*0.8, size.height()*0.8);
    window.resize(newSize);
#endif
    window.show();

    qDebug() << "Current Path: " << QDir::currentPath();

    return a.exec();
}
