#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <jibbs/mesh/AssimpMeshManager.h>
#include <jibbs/opengl/QtTextureManager.h>
#include <jibbs/opengl/OpenGLShaderProgram.h>

#include <glm/glm.hpp>
#include "matrices.h"

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QBasicTimer>
#include <QTimer>
#include <QOpenGLShaderProgram>

#include <QAccelerometer>
#include <QAccelerometerFilter>
#include <QAccelerometerReading>
#include <QOrientationSensor>
#include <QOrientationReading>
#include <QRotationSensor>

class shader;
class meshLoader;

#include <QOpenGLFunctions>

class OpenGLWindow : public QOpenGLWidget, private QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWidget* parent = nullptr);
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

    std::shared_ptr<QtTextureManager> _texManager = std::make_shared<QtTextureManager>("defaultTextureManager");
    std::shared_ptr<AssimpMeshManager> _meshManager = std::make_shared<AssimpMeshManager>();

    OpenGLShaderProgram _shaderProgram;
    meshLoader* _scene;

    matrices _pipeline;

    QTimer _timer;

    QAccelerometer _accelerometer;
    QOrientationSensor _orientation;

    QRotationSensor _rotationSensor;

    QImage _mainObbImg;
    std::unordered_map<QString, QByteArray> _obbByteArrays;

    float _bank=5.56f;
    float _pitch=-12.56f;
    std::vector<float> _bankHistory;
    std::vector<float> _pitchHistory;
    int _bankHistoryIdx = 0;

    QStringList _messageList;
};

#endif // OPENGLWINDOW_H
