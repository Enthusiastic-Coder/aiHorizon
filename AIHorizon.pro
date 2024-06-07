#-------------------------------------------------
#
# Project created by QtCreator 2015-05-12T14:31:59
#
#-------------------------------------------------

QT       += core gui sensors opengl openglwidgets


#need to reemove
 QT += network
 QT += core-private

TARGET = AIHorizon
CONFIG   += console

TEMPLATE = app

CONFIG += c++11

INCLUDEPATH += sdk/include
INCLUDEPATH += glm

include(sdk/jibbs.pri)
include(assimp2/assimp.pri)

include(AIHorizon.pri)

win32 {
scripts.files += scripts/__obbs/*.obb
scripts.path = $$DEPLOY_DIR/scripts
INSTALLS += scripts
}

aiHorizon.files += aiHorizon/*
aiHorizon.path = $$DEPLOY_DIR/aiHorizon
INSTALLS += aiHorizon

shaders.files += shaders/*
shaders.path = $$DEPLOY_DIR/shaders
INSTALLS += shaders

RESOURCES += \
    extra.qrc

DISTFILES += $$files("shaders/*", true) $$files("android/*", true)

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

