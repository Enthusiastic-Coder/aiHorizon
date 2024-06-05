#ifndef QTOPWINDOW_H
#define QTOPWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class OpenGLWindow;

class QTopWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit QTopWindow(QWidget *parent = nullptr);
    ~QTopWindow();

signals:

private:
    Ui::MainWindow *ui;
    OpenGLWindow* _openglWindow = nullptr;
};

#endif // QTOPWINDOW_H
