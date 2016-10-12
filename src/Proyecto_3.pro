#-------------------------------------------------
#
# Project created by QtCreator 2016-09-18T08:26:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Proyecto_3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    histogram.cpp \
    custommask.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    histogram.h \
    custommask.h

FORMS    += mainwindow.ui \
    histogram.ui \
    custommask.ui

DISTFILES +=
