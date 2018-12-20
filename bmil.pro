#-------------------------------------------------
#
# Project created by QtCreator 2018-11-05T21:01:16
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bmil
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    bmodel.cpp

HEADERS  += mainwindow.h \
    bmodel.h

FORMS    += mainwindow.ui
