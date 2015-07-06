#-------------------------------------------------
#
# Project created by QtCreator 2015-07-03T15:46:24
#
#-------------------------------------------------

CONFIG+=windeployqt
QMAKE_CFLAGS += -std=c99

INCLUDEPATH += ../../../../6IT
DEFINES += _6IT_QT
DEFINES +=_CRT_SECURE_NO_WARNINGS

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Subc_sample
TEMPLATE= app

SOURCES += main.cpp\
        mainwindow.cpp \
    ../../SubC_tester.c \
    ../../SubC_tester_main.c \
    ../../subc_tester_bios.cpp \
    ../../../../6IT/SubC_complete.c \
    rzqscrollarea.cpp \
    subc_worker.cpp

HEADERS  += mainwindow.h \
    rzqscrollarea.h \
    subc_worker.h

FORMS    += mainwindow.ui

RESOURCES += \
    assets.qrc


