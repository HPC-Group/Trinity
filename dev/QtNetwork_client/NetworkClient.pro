#-------------------------------------------------
#
# Project created by QtCreator 2015-08-22T09:59:31
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NetworkClient
TEMPLATE = app


SOURCES += main.cpp\
        client.cpp \
    myglwidget.cpp

HEADERS  += client.h \
    myglwidget.h

FORMS    += client.ui
