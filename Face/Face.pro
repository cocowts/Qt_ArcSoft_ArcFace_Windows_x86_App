#-------------------------------------------------
#
# Project created by QtCreator 2018-11-26T18:16:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Face
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        WigdetUI.cpp \
    widgetslot.cpp

HEADERS += \
        Wigdet.h \
    ArcSoft_ArcFace/inc/amcomdef.h \
    ArcSoft_ArcFace/inc/arcsoft_face_sdk.h \
    ArcSoft_ArcFace/inc/asvloffscreen.h \
    ArcSoft_ArcFace/inc/merror.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pic.qrc

win32: LIBS += -L$$PWD/ArcSoft_ArcFace/lib/Win32/ -llibarcsoft_face_engine

INCLUDEPATH += $$PWD/ArcSoft_ArcFace/inc
DEPENDPATH += $$PWD/ArcSoft_ArcFace/inc

win32 {
INCLUDEPATH += D:\OpenCV-MinGW-Build-OpenCV-3.3.1\include\
INCLUDEPATH += D:\OpenCV-MinGW-Build-OpenCV-3.3.1\include\opencv
INCLUDEPATH += D:\OpenCV-MinGW-Build-OpenCV-3.3.1\include\opencv2
LIBS += D:\OpenCV-MinGW-Build-OpenCV-3.3.1\bin\libopencv_*.dll
}
