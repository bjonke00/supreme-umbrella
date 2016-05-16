

TEMPLATE = app
TARGET = mosaic
DEPENDPATH += .
INCLUDEPATH += .
OBJECTS_DIR = ./obj
MOC_DIR = ./moc
CONFIG += release
QT += opengl core gui widgets

QMAKE_CXXFLAGS += -fPIC


LIBS += -lGLU32                           \



# Input
HEADERS += MainWindow.h                 \
           Tile.h			\
	   GLWidget.h


SOURCES += main.cpp                     \
           MainWindow.cpp               \
	   Tile.cpp			\
	   GLWidget.cpp
	   
	   
