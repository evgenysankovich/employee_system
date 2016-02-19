#-------------------------------------------------
#
# Project created by QtCreator 2016-02-16T10:48:34
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Employees
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialogaddemployee.cpp \
    database.cpp \
    workerbase.cpp

HEADERS  += mainwindow.h \
    dialogaddemployee.h \
    database.h \
    workerbase.h

FORMS    += mainwindow.ui \
    dialogaddemployee.ui
