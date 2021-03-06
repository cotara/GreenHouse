#-------------------------------------------------
#
# Project created by QtCreator 2020-02-25T14:48:35
#
#-------------------------------------------------

QT       += core gui serialport quick qml quickwidgets printsupport multimedia multimediawidgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GreenHouse
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
    axistag.cpp \
    customactuatorcontrol.cpp \
        main.cpp \
        greenhouse.cpp \
    passdialog.cpp \
    qcustomplot.cpp \
    serialsettings.cpp \
    console.cpp \
    greenhouselog.cpp \
    webcam.cpp \
    myplot.cpp

HEADERS += \
    axistag.h \
    customactuatorcontrol.h \
        greenhouse.h \
    passdialog.h \
    qcustomplot.h \
    serialsettings.h \
    console.h \
    greenhouselog.h \
    webcam.h \
    myplot.h

FORMS += \
    customactuatorcontrol.ui \
        greenhouse.ui \
    passdialog.ui \
    serialsettings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
RC_ICONS = grow.ico
