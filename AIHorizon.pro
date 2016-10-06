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

PROJECT = $(SDK_PROJECT)


include($$PROJECT/qMake/qMakeDestination.pri)
include($$PROJECT/qMake/qImportAssimp.pri)
include($$PROJECT/qMake/qImportGlee.pri)
include($$PROJECT/qMake/qImportGlm.pri)
include(AIHorizon.pri)

INCLUDEPATH += $$PROJECT/include

win32 {
    LIBS +=-L"C:/Program Files (x86)/OpenAL 1.1 SDK/libs/Win32"
    LIBS += -l"opengl32" -l"user32" -l"gdi32"
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
    shaders/vertex.vert

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

