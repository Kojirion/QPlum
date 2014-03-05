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
QMAKE_CXXFLAGS += -gdwarf-3


SOURCES += main.cpp\
        MainWindow.cpp \
    NodeModel.cpp \
    ElementModel.cpp \
    Node.cpp \
    Element.cpp \
    Scene.cpp

HEADERS  += MainWindow.hpp \
    NodeModel.hpp \
    ElementModel.hpp \
    Node.hpp \
    Element.hpp \
    Scene.hpp

FORMS    += mainwindow.ui
