#include "QTopWindow.h"

#include "openglwindow.h"

#include "./ui_MainWindow.h"

#include <QMessageBox>


QTopWindow::QTopWindow(QWidget *parent)
    : QMainWindow{parent} , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _openglWindow = new OpenGLWindow;

    setCentralWidget(_openglWindow);

    connect( ui->actionPrivacy_Policy, &QAction::triggered, this, [this](bool checked) {

        QMessageBox::information(this, "Privacy Policy", "Please visit https://enthusiastic-coder.github.io/aihorizon-privacy-policy/");
    });
}

QTopWindow::~QTopWindow()
{
    delete ui;
}
