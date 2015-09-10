#-------------------------------------------------
#
# Project created by QtCreator 2015-06-25T10:50:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtFrontend
TEMPLATE = app


SOURCES += main.cpp\
        qtfrontendwindow.cpp \
    renderlabel.cpp \
    myglwidget.cpp

HEADERS  += qtfrontendwindow.h \
    renderlabel.h \
    myglwidget.h

FORMS    += qtfrontendwindow.ui
