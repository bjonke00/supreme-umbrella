#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "Tile.h"
#include "glwidget.h"
#include <QtGui>
#include <QtWidgets>
#include <QMainWindow>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
     //constructor
    MainWindow(QWidget *parent = 0);

     //slots
public slots:
     void    s_loadTiles   ();

private:
     void	createActions	();
     void	createMenus	();
     void	createWidgets	();

     //widgets
     QPushButton    *m_LoadButton;
     QPushButton    *m_playPause;
     QPushButton    *m_reset;
     QSlider        *m_slider;
     QSpinBox       *m_spinBox;
     vector<Tile>   m_tiles;
     GLWidget       *m_glwidget;

     //actions
     QAction*     m_actionLoad;
     QAction*     m_actionQuit;
     QMenu*       m_menuFile;

};

#endif // MAINWINDOW_H
