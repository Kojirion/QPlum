#-------------------------------------------------
#
# Project created by QtCreator 2014-02-19T18:47:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Plum_gui
TEMPLATE = app

CONFIG += c++11


SOURCES += main.cpp\
        MainWindow.cpp \
    NodesModel.cpp

HEADERS  += MainWindow.hpp \
    NodesModel.hpp

FORMS    += mainwindow.ui
