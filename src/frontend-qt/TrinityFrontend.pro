#-------------------------------------------------
#
# Project created by QtCreator 2016-02-18T14:23:49
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrinityFrontend
TEMPLATE = app

LIBS += -lopengl32

SOURCES +=\
            main.cpp\
            window.cpp \
    openglwidget.cpp

HEADERS  += \
        window.h \
    openglwidget.h


FORMS    += window.ui
