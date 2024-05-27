#-------------------------------------------------
#
# Project created by QtCreator 2015-05-12T14:31:59
#
#-------------------------------------------------

QT       += core gui sensors opengl

TARGET = AIHorizon
CONFIG   += console

TEMPLATE = app

CONFIG += c++11

INCLUDEPATH += sdk/include
INCLUDEPATH += glm

include(sdk/qMake/qMakeDestination.pri)
include(assimp/assimp.pri)

#win32 {
#    SOURCES += sdk/include/GLee.c
#    HEADERS += sdk/include/GLee.h
#}

include(AIHorizon.pri)

#win32 {
#   LIBS += -l"opengl32"
#}


aiHorizon.files += aiHorizon/*
aiHorizon.path = $$DEPLOY_DIR/aiHorizon
INSTALLS += aiHorizon

shaders.files += shaders/*
shaders.path = $$DEPLOY_DIR/shaders
INSTALLS += shaders

RESOURCES +=

DISTFILES += $$files("shaders/*", true) $$files("android/*", true)

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

