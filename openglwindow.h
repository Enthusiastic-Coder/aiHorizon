#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <glm/glm.hpp>
#include "matrices.h"

#include <QOpenGLWindow>
#include <QBasicTimer>
#include <QTimer>

#include <QAccelerometer>
#include <QAccelerometerFilter>
#include <QAccelerometerReading>
#include <QOrientationSensor>
#include <QOrientationReading>

class shader;
class meshLoader;

#include <QOpenGLFunctions>

class OpenGLWindow : public QOpenGLWindow, private QOpenGLFunctions
{
    Q_OBJECT
public:
    OpenGLWindow();
    ~OpenGLWindow();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *e) Q_DECL_OVERRIDE;

private:
    shader* _shaderProgram;
    meshLoader* _scene;

    matrices _pipeline;

    QTimer _timer;

    QAccelerometer _accelerometer;
    QOrientationSensor _orientation;

    std::vector<float> _bankHistory;
    std::vector<float> _pitchHistory;
    int _bankHistoryIdx = 0;

    QString _mainOBBPath;
    QString _patchOBBPath;
    QString _messageText;
    QString _messageText2;
};

#endif // OPENGLWINDOW_H
