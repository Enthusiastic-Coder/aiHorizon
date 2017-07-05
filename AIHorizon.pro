#-------------------------------------------------
#
# Project created by QtCreator 2015-05-12T14:31:59
#
#-------------------------------------------------

QT       += core gui sensors

TARGET = AIHorizon
CONFIG   += console

TEMPLATE = app

CONFIG += c++11

INCLUDEPATH += sdk/include
INCLUDEPATH += glm

include(sdk/qMake/qMakeDestination.pri)
include(assimp/assimp.pri)

win32 {
    SOURCES += sdk/include/GLee.c
    HEADERS += sdk/include/GLee.h
}

include(AIHorizon.pri)

win32 {
   LIBS += -l"opengl32"
}


aiHorizon.files += aiHorizon/*
aiHorizon.path = $$DEPLOY_DIR/aiHorizon
INSTALLS += aiHorizon

shaders.files += shaders/*
shaders.path = $$DEPLOY_DIR/shaders
INSTALLS += shaders

RESOURCES +=

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    shaders/fragment.frag \
    shaders/vertex.vert \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

