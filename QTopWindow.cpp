#include "QTopWindow.h"

#include "openglwindow.h"

#include "./ui_MainWindow.h"


QTopWindow::QTopWindow(QWidget *parent)
    : QMainWindow{parent} , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _openglWindow = new OpenGLWindow;

    setCentralWidget(_openglWindow);
}

QTopWindow::~QTopWindow()
{
    delete ui;
}
