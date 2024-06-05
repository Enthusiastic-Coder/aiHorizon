#include "QTopWindow.h"

#include "openglwindow.h"

#include "./ui_MainWindow.h"


QTopWindow::QTopWindow(QWidget *parent)
    : QMainWindow{parent} , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _openglWindow = new OpenGLWindow;

    setCentralWidget(_openglWindow);

    connect( ui->actionPrivacy_Policy, &QAction::triggered, this, [this](bool checked) {

        int o;
        o = 3;
    });
}

QTopWindow::~QTopWindow()
{
    delete ui;
}
