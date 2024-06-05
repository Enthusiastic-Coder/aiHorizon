#include <QApplication>

#include "QTopWindow.h"
#include <QSurfaceFormat>

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
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    fmt.setAlphaBufferSize(8);
    fmt.setStencilBufferSize(8);
    QSurfaceFormat::setDefaultFormat(fmt);

    QApplication a(argc, argv);

    QTopWindow window;
#ifndef ANDROID
    QSize size = QGuiApplication::screens()[0]->availableSize();
    QSize newSize(size.width()*0.8, size.height()*0.8);
    window.resize(newSize);
#endif
    window.show();

    qDebug() << "Current Path: " << QDir::currentPath();

    return a.exec();
}
