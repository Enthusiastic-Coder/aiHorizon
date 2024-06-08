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

    _openglWindow->setFocus();

    connect( ui->actionPrivacy_Policy, &QAction::triggered, this, [this](bool checked) {

        Q_UNUSED(checked)
        QMessageBox::information(this, "Privacy Policy", "Please visit https://enthusiastic-coder.github.io/aihorizon-privacy-policy/");
    });
}

QTopWindow::~QTopWindow()
{
    delete ui;
}
