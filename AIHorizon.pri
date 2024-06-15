SOURCES += main.cpp \
    openglwindow.cpp \
    camera.cpp \
    vector3d.cpp \
    shader.cpp \
    matrices.cpp \
    $$PWD/openglcontextTest.cpp

HEADERS += \
    openglwindow.h \
    meshData.h \
    camera.h \
    vector3d.h \
    shader.h \
    matrices.h \
    $$PWD/openglcontextTest.h



FORMS += \
    MainWindow.ui

HEADERS += \
    QTopWindow.h \
    madgwick\MadgwickAHRS.h

SOURCES += \
    QTopWindow.cpp \
    madgwick\MadgwickAHRS.cpp
