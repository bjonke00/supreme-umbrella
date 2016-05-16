// ======================================================================
//
// GLWidget.h - GL widget class
//
// ======================================================================

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtGui>
#include <QtOpenGL>
#include <GL/glu.h>
#include "Tile.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
     GLWidget(QWidget *parent = 0);
    ~GLWidget();

    void  setTiles(vector<Tile> &);
    void  drawTiles();
    //void  rotatexTiles();
    //void  rotateyTiles();
    //void  scaleTiles();
    void  setTimer();

public slots:
    void setCentroid();
    void setScale();
    void setRotate();
    void setSpeed(int);
    void setPlay();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    vector<Tile>	m_tiles;
    QTimer *timer;
    float   angle;
    float   scale;
    float   angleStep;
    float   scaleStep;
    bool    expand;
    bool    playset;
    bool    scaleset;
    bool    rotateset;
    bool    centroidset;

};

#endif	// GLWIDGET_H
