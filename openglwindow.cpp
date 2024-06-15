#include "openglwindow.h"

#include <camera.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <shader.h>
#include <AndroidAssetsTransfer.inl>

#include <QKeyEvent>
#include <QPainter>
#include <QCoreApplication>
#include "jibbs/mesh/meshloader.h"
#include "openglcontextTest.h"
#include <QDir>
#include <QResource>

#include <jibbs/android/assetpack.h>

#include "madgwick/MadgwickAHRS.h"


camera cam(vector3d(-3,10,25));
Madgwick madgwick;

OpenGLWindow::OpenGLWindow(QWidget *parent) :QOpenGLWidget{parent}
{
    _bankHistory.resize(5);
    _pitchHistory.resize(_bankHistory.size());

    std::vector<QString> files = {
                                  "main",
                                  "patch",
                                  "extra",
                                  };

    AssetPack assetPack("com/enthusiasticcoder/aihorizon");

    for(const QString &file : files)
    {
        QString pack;
        QString filename;
#ifdef Q_OS_ANDROID
        pack = file;
#else
        pack = "scripts";
#endif
        filename = file + ".obb";
        _obbByteArrays[file] = assetPack.getDataFromAsset(pack, filename);

        if(_obbByteArrays[file].isNull())
        {
            _messageList << QString("%1-%2").arg(file, "Null");
        }

#ifdef __RESOURCES__CHECK__
        AssetLocation loc = assetPack.getAssetLocation(pack, filename);
        if( !loc.path.isEmpty())
            _messageList << QString("%1->%2").arg(file, loc.path.right(40));
#endif
    }

    auto registerResource = [](QByteArray data, QString prefix="") {
        return QResource::registerResource(reinterpret_cast<const uchar*>(data.constData()), prefix);
    };

    for(const auto& resource: _obbByteArrays)
        registerResource(resource.second, resource.first=="extra"? "/hello":"");

#ifdef __RESOURCES__CHECK__
    {
        QFile osmFile(":/images/adsbfl.png");
        if( osmFile.open(QIODevice::ReadOnly))
        {
            _mainObbImg = QImage::fromData(osmFile.readAll());
        }
        else
            _messageList << "osmtiles not found";
    }

    {
        QFile bus119(":/data/Routes/outbound/bus/119.txt");
        if( bus119.open(QIODevice::Text|QIODevice::ReadOnly))
        {
            _messageList<< "Bus119:" + bus119.read(40);
        }
        else
            _messageList << "Bus 119 NOT FOUND";
    }

    {
        QFile mainCpp(":/hello/extra/main.cpp");
        if( mainCpp.open(QIODevice::Text|QIODevice::ReadOnly))
        {
            _messageList<< "mainCpp:" + mainCpp.read(40);
        }
        else
            _messageList << "mainCpp NOT FOUND";
    }
#endif

    _elapsedTimer.start();
}

OpenGLWindow::~OpenGLWindow()
{
    delete _scene;
    _scene = 0;
}

void OpenGLWindow::initializeGL()
{
    QOpenGLFunctions::initializeOpenGLFunctions();

    _shaderProgram.loadSrc( readAllRP("shaders/vertex.vert"),
                           readAllRP("shaders/fragment.frag"));

    _scene = new meshLoader(_texManager, _meshManager);
    _scene->load(ResourcePath("aiHorizon/output.obj"));

    const char* version = (const char*)glGetString(GL_VERSION);

    doTest(context()->functions());

    glEnable(GL_DEPTH_TEST);
    glClearColor(0,0,1,1);
    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timer.start(100);
    _accelerometer.setActive(true);
    _orientation.setActive(true);
    _rotationSensor.setActive(true);
    _pressureSensor.setActive(true);
    _gyroSensor.setActive(true);
    _compassSensor.setActive(true);
    _magnoSensor.setActive(true);

}

void OpenGLWindow::paintGL()
{
    // if(!isExposed())
    //     return;

    qreal ax ={}, ay={}, az={};
    qreal mx ={}, my={}, mz={};
    qreal gx ={}, gy={}, gz={};

    QAccelerometerReading* accReading = _accelerometer.reading();
    QOrientationReading* o = _orientation.reading();

    if( accReading && o)
    {
        qreal x, y, z;

        if( o->orientation() == QOrientationReading::RightUp )
        {
            y = accReading->x();
            x = -accReading->y();
            z = accReading->z();
        }
        else if( o->orientation() == QOrientationReading::LeftUp )
        {
            y = -accReading->x();
            x = accReading->y();
            z = accReading->z();
        }
        else
        {
            x = accReading->x();
            y = accReading->y();
            z = accReading->z();
        }

        ax = accReading->x();
        ay = accReading->y();
        az = accReading->z();

        _bank = atan2(x,y) / 3.1415 * 180;
        _pitch = atan2(z,y) / 3.1415 * 180;
    }

    if( _bankHistoryIdx == _bankHistory.size())
        _bankHistoryIdx = 0;

    _pitchHistory[_bankHistoryIdx] = _pitch;
    _bankHistory[_bankHistoryIdx++] = _bank;

    _bank = 0.0f;

    for(auto value:_bankHistory)
        _bank += value;

    _bank /= _bankHistory.size();

    _pitch = 0.0f;
    for(auto value:_pitchHistory)
        _pitch += value;

    _pitch /= _pitchHistory.size();

    const auto& meshes = _scene->getMeshes();

    glEnable(GL_DEPTH_TEST);

    glClear(GL_DEPTH_BUFFER_BIT);

    _shaderProgram.use();

    _pipeline.matrixMode(matrixModes::MODEL_MATRIX);
    _pipeline.loadIdentity();
    _pipeline.translate(0,0, -5.5);

    _pipeline.matrixMode(matrixModes::VIEW_MATRIX);
    _pipeline.loadIdentity();

    glUniform3f(glGetUniformLocation(_shaderProgram.getProgramID(), "lightPos"), 0, 1, 2 );

    glUniform3f(glGetUniformLocation(_shaderProgram.getProgramID(), "cameraPosition"),
                cam.getLocation().x,cam.getLocation().y,cam.getLocation().z);

    for( const auto& mesh: meshes)
    {
        _pipeline.matrixMode(matrixModes::MODEL_MATRIX);
        _pipeline.loadIdentity();

        if(o &&
                (o->orientation() == QOrientationReading::TopUp
                 || o->orientation() == QOrientationReading::FaceUp
                 || o->orientation() == QOrientationReading::FaceDown)
            )
            _pipeline.translate(0,0, -9.0);
        else
            _pipeline.translate(0,0, -5.5);

        if( mesh->name() == "secondary_ai_color_Disk")
        {
            _pipeline.rotateZ(-int(_bank));
#ifdef ANDROID
            _pipeline.translate(0, (_pitch-20)/20,0);;
#else
            _pipeline.translate(0, _pitch/20,0);
#endif
        }

        if( mesh->name() == "secondary_ai_color_Housing__Orange")
        {
            _pipeline.rotateZ(-int(_bank));
        }

        _pipeline.updateMatrices(_shaderProgram.getProgramID());

        mesh.draw(_shaderProgram);
    }

    _shaderProgram.useDefault();

    QPainter p(this);
    // p.fillRect(QRect{0,0,width(), height()}, Qt::blue);
    QFont font("Verdana", 14);
    QFontMetrics fm(font);
    p.setFont(font);
    p.setPen(Qt::white);

    int count = 1;
    QStringList messageList;

    QString str = QString("Pitch:%1").arg(_pitch,4, 'f', 1, '0');
    messageList << str;

    str = QString("Bank:%1").arg(_bank,4, 'f', 1, '0');
    messageList << str;

    QRotationReading *rotReading = _rotationSensor.reading();

    if(rotReading)
    {
        messageList << QString("Rot:{%1, %2, %3}").arg(rotReading->x()).arg(rotReading->y()).arg(rotReading->z());
    }

    QPressureReading *pressureReading = _pressureSensor.reading();

    if(pressureReading)
    {
        messageList << QString("Pressure:{%1}").arg(pressureReading->pressure());
        messageList << QString("Temp:{%1}").arg(pressureReading->temperature());
    }

    QGyroscopeReading *gyroReading = _gyroSensor.reading();

    if( gyroReading)
    {
        messageList << QString("Gyro:{%1, %2, %3}").arg(gyroReading->x()).arg(gyroReading->y()).arg(gyroReading->z());

        gx = gyroReading->x();
        gy = gyroReading->y();
        gz = gyroReading->z();
    }

    QCompassReading *compassReading = _compassSensor.reading();

    if( compassReading)
    {
        messageList << QString("Compass_Azimuth:{%1}").arg(compassReading->azimuth());
        messageList << QString("Compass_Calib:{%1}").arg(compassReading->calibrationLevel());
    }

    QMagnetometerReading* magnoReading = _magnoSensor.reading();

    if( magnoReading)
    {
        messageList << QString("Mag:{%1, %2, %3}").arg(magnoReading->x()).arg(magnoReading->y()).arg(magnoReading->z());
        messageList << QString("Mag_Calib:{%1}").arg(magnoReading->calibrationLevel());

        mx = magnoReading->x();
        my = magnoReading->y();
        mz = magnoReading->z();
    }

    qint64 currentTime = _elapsedTimer.elapsed();

    double dt = (currentTime - _lastTime) / 1000.0; // elapsed time in milliseconds
    _lastTime = currentTime;

    madgwick.update(gx, gy, gz, ax, ay, az, mx, my, mz);

    messageList << QString("Magwick{%1, %2, %3}")
                       .arg(madgwick.getPitch())
                       .arg(madgwick.getYaw())
                       .arg(madgwick.getRoll());

    auto displayMsg = [&count,&p,&fm](const auto& msgList) {
        for(auto& line:msgList)
        {
            p.drawText(0, count++* fm.height(), line);
        }
    };

    displayMsg(messageList);
    displayMsg(_messageList);

    //p.drawImage(5, count * fm.height(), _mainObbImg);

}

void OpenGLWindow::resizeGL(int w, int h)
{
    QOpenGLWidget::resizeGL(w, h);
    _pipeline.perspective(75.0f, qreal(w) / qreal(h), 1.0f, 1000.f);
}

void OpenGLWindow::keyPressEvent(QKeyEvent *e)
{
    if( e->key() == Qt::Key_Escape)
        close();

    if( e->key() == Qt::Key_Left)
        _bank+=5;

    if( e->key() == Qt::Key_Right)
        _bank-=5;

    if( e->key() == Qt::Key_Up)
        _pitch +=5;

    if( e->key() == Qt::Key_Down)
        _pitch -=5;

    QOpenGLWidget::keyPressEvent(e);
}

void OpenGLWindow::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position

}

void OpenGLWindow::mouseReleaseEvent(QMouseEvent *)
{
//    // Mouse release position - mouse press position
//        QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

//        // Rotation axis is perpendicular to the mouse position difference
//        // vector
//        QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

//        // Accelerate angular speed relative to the length of the mouse sweep
//        qreal acc = diff.length() / 100.0;

//        // Calculate new rotation axis as weighted sum
//        rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

//        // Increase angular speed
//        angularSpeed += acc;

}

void OpenGLWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {

    }
}

void OpenGLWindow::timerEvent(QTimerEvent *)
{
           update();

}
