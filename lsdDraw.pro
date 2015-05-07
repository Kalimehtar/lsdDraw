#-------------------------------------------------
#
# Project created by QtCreator 2015-05-05T21:19:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lsdDraw
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

# remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2

# add the desired -O3 if not present
QMAKE_CXXFLAGS_RELEASE *= -O3
QMAKE_CXXFLAGS += -O3
