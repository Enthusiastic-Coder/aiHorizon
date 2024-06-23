#include <QApplication>

#include "QTopWindow.h"
#include <QSurfaceFormat>

#include <QSize>
#include <QScreen>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <QDir>

#ifdef Q_OS_ANDROID
#include <QJniObject>

void keepScreenOn() {

    QNativeInterface::QAndroidApplication::runOnAndroidMainThread([] {

    QJniObject activity = QNativeInterface::QAndroidApplication::context();
        QJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");
        window.callMethod<void>("addFlags", "(I)V", 0x00000080); // FLAG_KEEP_SCREEN_On
    });
}
#endif


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

#ifdef Q_OS_ANDROID
    keepScreenOn();
#endif

    QTopWindow window;
#ifndef ANDROID
    const QList<QScreen *>& screens = QGuiApplication::screens();
    QSize size = screens.at(0)->availableSize();
    QSize newSize(size.width()*0.8, size.height()*0.8);
    window.resize(newSize);
#endif
    window.show();

    qDebug() << "Current Path: " << QDir::currentPath();

    return a.exec();
}
