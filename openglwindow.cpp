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


camera cam(vector3d(-3,10,25));


OpenGLWindow::OpenGLWindow()
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

        _messageList << QString("%1-%2").arg(file).arg( _obbByteArrays[file].isNull() ? "Null": "LoadedOK");

        AssetLocation loc = assetPack.getAssetLocation(pack, filename);
        if( !loc.path.isEmpty())
            _messageList << QString("%1->%2").arg(file, loc.path.right(40));
    }

    auto registerResource = [](QByteArray data, QString prefix="") {
        return QResource::registerResource(reinterpret_cast<const uchar*>(data.constData()), prefix);
    };

    for(const auto& resource: _obbByteArrays)
        registerResource(resource.second, resource.first=="extra"? "/hello":"");

    {
        QFile osmFile(":/osmtiles/osm/7/64_42.png");
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


    QPainter p(this);
    p.fillRect(QRect{0,0,width(), height()}, Qt::blue);
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

    int count = 6;

    for(auto& line:_messageList)
    {
        p.drawText(0, count++* fm.height(), line);
    }

    p.drawImage(5, count * fm.height(), _mainObbImg);


    const auto& meshes = _scene->getMeshes();

    p.beginNativePainting();

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
            _pipeline.rotateZ(-int(bank));
#ifdef ANDROID
            _pipeline.translate(0, (pitch-20)/20,0);;
#else
            _pipeline.translate(0, pitch/20,0);
#endif
        }

        if( mesh->name() == "secondary_ai_color_Housing__Orange")
        {
            _pipeline.rotateZ(-int(bank));
        }

        _pipeline.updateMatrices(_shaderProgram.getProgramID());

        mesh.draw(_shaderProgram);
    }

    _shaderProgram.useDefault();

    p.endNativePainting();
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
