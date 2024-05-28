#include "openglwindow.h"

#include <camera.h>
#include "aimeshloader.h"

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
#include "openglcontextTest.h"
#include <QDir>

#ifdef Q_OS_ANDROID

#include <QJniObject>
#include <QJniEnvironment>

/*
QString getAssetPath(const QString &packName) {

    QJniObject context = QNativeInterface::QAndroidApplication::context();
    if (!context.isValid()) {
        return "Failed to get Android context";
    }

    QJniObject assetPackManager = QJniObject::callStaticObjectMethod(
        "com/google/android/play/core/assetpacks/AssetPackManagerFactory",
        "getInstance",
        "(Landroid/content/Context;)Lcom/google/android/play/core/assetpacks/AssetPackManager;",
        context.object()
        );

    if (!assetPackManager.isValid()) {
        return "Failed to get AssetPackManager instance";
    }

    QJniObject packNameJni = QJniObject::fromString(packName);
    QJniObject location = assetPackManager.callObjectMethod(
        "getPackLocation",
        "(Ljava/lang/String;)Lcom/google/android/play/core/assetpacks/AssetPackLocation;",
        packNameJni.object<jstring>()
        );

    if (!location.isValid()) {
        return "Failed to get asset pack location";
    }

    QJniObject path = location.callObjectMethod("assetsPath", "()Ljava/lang/String;");
    return path.toString();
}*/

QString getAssetPackPath(const QString &packName, const QString &assetName) {
     QJniObject context = QNativeInterface::QAndroidApplication::context();
    QJniObject assetPackHelper = QJniObject(
        "com/enthusiasticcoder/aihorizon/AssetPackHelper",
        "(Landroid/content/Context;)V",
        context.object()
        );

    if (assetPackHelper.isValid()) {
        QJniObject packNameJni = QJniObject::fromString(packName);
        QJniObject assetNameJni = QJniObject::fromString(assetName);
        QJniObject assetPath = assetPackHelper.callObjectMethod(
            "getAssetPackPath",
            "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;",
            packNameJni.object<jstring>(),
            assetNameJni.object<jstring>()
            );

        if (assetPath.isValid()) {
            return assetPath.toString();
        } else {
            return "Failed to get asset path for " + packName + assetName;
        }
    } else {
        return "Failed to create AssetPackHelper instance";
    }
    return "";
}

#endif

QString checkAndAccessObbFiles() {

#ifdef Q_OS_ANDROID
    QString mainPackPath = getAssetPackPath("main","main.obb");

    if (mainPackPath.isEmpty()) {
        return "Assets from mainpack not found!";
    } else {
        return mainPackPath;
        // Access files in mainPackPath
    }
#else
    return "Windows";
#endif
}



camera cam(vector3d(-3,10,25));


OpenGLWindow::OpenGLWindow()
{
    _bankHistory.resize(5);
    _pitchHistory.resize(_bankHistory.size());
    _OBBPath = checkAndAccessObbFiles();
}

OpenGLWindow::~OpenGLWindow()
{
    delete _shaderProgram;
    _shaderProgram =0;

    delete _scene;
    _scene = 0;
}

void OpenGLWindow::initializeGL()
{
    QOpenGLFunctions::initializeOpenGLFunctions();
//    copyAssets(true);
    _shaderProgram= new shader("shaders/vertex.vert", "shaders/fragment.frag");
    _scene = new AIMeshLoader("aiHorizon/output.obj");

    const char* version = (const char*)glGetString(GL_VERSION);

    doTest(this);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0,0,1,1);
    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timer.start(100);
    _accelerometer.setActive(true);
    _orientation.setActive(true);


}

void OpenGLWindow::paintGL()
{
    if(!isExposed())
        return;

    float bank=5.56f;
    float pitch=-12.56f;

    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    QPainter p;
//    p.begin(this);
//    p.save();
//    p.setPen(Qt::white);
//    p.drawText(100,100,"HELLO");
//    p.restore();
//    p.end();

    _shaderProgram->use();

    _pipeline.matrixMode(matrixModes::MODEL_MATRIX);
    _pipeline.loadIdentity();
    _pipeline.translate(0,0, -5.5);

    _pipeline.matrixMode(matrixModes::VIEW_MATRIX);
    _pipeline.loadIdentity();

    glUniform3f(glGetUniformLocation(_shaderProgram->getProgramId(), "lightPos"), 0, 1, 2 );

    glUniform3f(glGetUniformLocation(_shaderProgram->getProgramId(), "cameraPosition"),
                    cam.getLocation().x,cam.getLocation().y,cam.getLocation().z);

    QAccelerometerReading* reading = _accelerometer.reading();
    QOrientationReading* o = _orientation.reading();


    if( reading && o)
    {
        qreal x, y, z;

        if( o->orientation() == QOrientationReading::RightUp )
        {
            y = reading->x();
            x = -reading->y();
            z = reading->z();
        }
        else if( o->orientation() == QOrientationReading::LeftUp )
        {
            y = -reading->x();
            x = reading->y();
            z = reading->z();
        }
        else
        {
            x = reading->x();
            y = reading->y();
            z = reading->z();
        }

        bank = atan2(x,y) / 3.1415 * 180;
        pitch = atan2(z,y) / 3.1415 * 180;
    }

    if( _bankHistoryIdx == _bankHistory.size())
        _bankHistoryIdx = 0;

    _pitchHistory[_bankHistoryIdx] = pitch;
    _bankHistory[_bankHistoryIdx++] = bank;

    bank = 0.0f;

    for(auto value:_bankHistory)
        bank += value;

    bank /= _bankHistory.size();

    pitch = 0.0f;
    for(auto value:_pitchHistory)
        pitch += value;

    pitch /= _pitchHistory.size();

    std::vector<mesh*>& meshes = _scene->getMeshes();

    for( size_t i =0 ; i < meshes.size(); ++i)
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

        if( meshes[i]->name() == "secondary_ai_color_Disk")
        {
            _pipeline.rotateZ(-int(bank));
#ifdef ANDROID
            _pipeline.translate(0, (pitch-20)/20,0);;
#else
            _pipeline.translate(0, pitch/20,0);
#endif
        }

        if( meshes[i]->name() == "secondary_ai_color_Housing__Orange")
        {
            _pipeline.rotateZ(-int(bank));
        }

        _pipeline.updateMatrices(_shaderProgram->getProgramId());

        meshes[i]->draw(_shaderProgram->getProgramId());
    }

    _shaderProgram->release();

//    QPainter p(this);
p.begin(this);
    QFont font("Verdana", 14);
    QFontMetrics fm(font);
    p.setFont(font);
    p.setPen(Qt::white);
    QString str = QString("Pitch:%1").arg(pitch,4, 'f', 1, '0');
    p.drawText(0,fm.height(), str);
    str = QString("Bank:%1").arg(bank,4, 'f', 1, '0');
    p.drawText(0,2* fm.height(), str);

    p.drawText(0, 3* fm.height(), "currentPath : " + QDir::currentPath());
    p.drawText(0, 4* fm.height(), "homePath : " + QDir::homePath());
    p.drawText(0, 5* fm.height(), "applicationDirPath : " + QCoreApplication::applicationDirPath());
    p.drawText(0, 6* fm.height(), "obb Path : " + _OBBPath);

    const QString pathExists = QFile::exists(_OBBPath)  ? "Found" : "Not Found";
    p.drawText(0, 7* fm.height(), "obb Path exists: " + pathExists );

p.end();
}

void OpenGLWindow::resizeGL(int w, int h)
{
    QOpenGLWindow::resizeGL(w, h);
    _pipeline.perspective(75.0f, qreal(w) / qreal(h), 1.0f, 1000.f);
}

void OpenGLWindow::keyPressEvent(QKeyEvent *e)
{
    if( e->key() == Qt::Key_Escape)
        close();

    QOpenGLWindow::keyPressEvent(e);
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
