

TEMPLATE = app
TARGET = mosaic_morphing
DEPENDPATH += .
INCLUDEPATH += .
OBJECTS_DIR = ./obj
MOC_DIR = ./moc
CONFIG += release
QT += opengl core gui widgets

QMAKE_CXXFLAGS += -fPIC


# Input
HEADERS += MainWindow.h                 \
           Tile.h			\
	   GLWidget.h


SOURCES += main.cpp                     \
           MainWindow.cpp               \
	   Tile.cpp			\
	   GLWidget.cpp
	   
	   
