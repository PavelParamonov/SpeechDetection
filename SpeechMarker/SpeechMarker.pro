#-------------------------------------------------
#
# Project created by QtCreator 2015-05-19T10:38:51
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpeechMarker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    renderarea.cpp \
    workerwavfilereader.cpp \
    workerwavformwrecalculation.cpp

HEADERS  += mainwindow.h \
    renderarea.h \
    workerwavfilereader.h \
    workerwavformwrecalculation.h
