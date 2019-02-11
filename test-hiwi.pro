#-------------------------------------------------
#
# Project created by QtCreator 2019-01-07T16:04:37
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test-hiwi
DESTDIR = "C:\HIWI\Release"
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

INCLUDEPATH +=.
INCLUDEPATH += ../IAW/core/trunk
INCLUDEPATH += "C:\HIWI\Release/boost/1.66.0/include"
INCLUDEPATH += "C:\HIWI\lib\opencv-3.4.5\include"


LIBS+=-L"C:\HIWI\Release"
LIBS+=-L"C:\HIWI\Release\zlib-1.2.11\msvc-2015-x64\lib"
LIBS+=-L"C:\HIWI\lib\opencv-3.4.5\x64\vc15\lib" -lopencv_world345

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    histogram.cpp \
    imgfunctions.cpp \
    window_histmatching.cpp

HEADERS += \
        mainwindow.h \
    histogram.h \
    convfunktion.h \
    imgfunctions.h \
    window_histmatching.h

FORMS += \
        mainwindow.ui \
    window_histmatching.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
