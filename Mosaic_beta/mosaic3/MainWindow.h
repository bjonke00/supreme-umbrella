// ======================================================================
//
// MainWindow.h - Main window widget class
//
// ======================================================================

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "Tile.h"
#include "GLWidget.h"

// forward declaration
class GLWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // constructor
    MainWindow(QWidget *parent = 0);

public slots:
    // slots
    void s_loadTiles();		// read tile geometry from file

private:
    void	createActions	();
    void	createMenus	();
    void	createWidgets	();

    //mouse
    QSlider *createSlider();                                            //sliders
    QSlider *xSlider;
    QSlider *ySlider;
    QSlider *zSlider;

    GLWidget	*m_glwidget;	// widget in which to draw

    // widgets
    QPushButton *m_load;
    QPushButton *m_play;
    QPushButton *m_reset;
    QCheckBox   *m_spiral;
    QCheckBox   *m_space;

    // actions
    QAction	*m_actionLoad;
    QAction	*m_actionQuit;
};

#endif	// MAINWINDOW_H
