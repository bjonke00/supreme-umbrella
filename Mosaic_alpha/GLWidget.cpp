// ======================================================================
//
// GLWidget.cpp - GL widget class
//
// ======================================================================

#include "GLWidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    m_tiles.clear();
    angle = 0.0f;
    scale = 1.0f;
    angleStep = 1.0f;
    scaleStep = 0.998f; //x^360=0.5 => x=10^(log10(0.5)/360)
    expand    = false;
    playset      = false;
    scaleset     = false;
    rotateset    = false;
    centroidset  = false;
}



GLWidget::~GLWidget()
{}

//slot to set scale
void GLWidget::setScale()
{
    scaleset = !scaleset;

}


void GLWidget::setRotate()
{
    rotateset = !rotateset;

}

void GLWidget::setCentroid()
{
    centroidset = !centroidset;
    updateGL();


}

void GLWidget::setSpeed(int speed)
{

    switch(speed){
    case 1:
        scaleStep = 0.998f;
        angleStep = 1;
        break;
    case 2:
        scaleStep = 0.997f;
        angleStep = 1.5;
        break;
    case 3:
        scaleStep = 0.996f;
        angleStep = 2;
        break;
    case 4:
        scaleStep = 0.995f;
        angleStep = 2.5;
        break;
    case 5:
        scaleStep = 0.994f;
        angleStep = 3;
        break;
    case 6:
        scaleStep = 0.993f;
        angleStep = 4;
        break;
    case 7:
        scaleStep = 0.992f;
        angleStep = 4.5;
        break;
    case 8:
        scaleStep = 0.991f;
        angleStep = 5;
        break;
    default:
        break;
    }
}

void GLWidget::setPlay()
{
    playset = !playset;
    if(playset)
      setTimer();
    else
      timer->stop();
}

//slot to set timer for animation
void GLWidget::setTimer()
{
    timer = new QTimer(this);
    timer->start(15);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));

}


void GLWidget::initializeGL()
{
    qglClearColor(QColor(0., 0., 0., 1.0));

    // enable smooth points
    glEnable(GL_POINT_SMOOTH);

    // enable depth test
    glEnable(GL_DEPTH_TEST);

}


void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    drawTiles();  

    if (scaleset)
    {
        if (scale <= 0.5f)
            expand = true;
        if (scale >= 1.0f)
            expand = false;

        if (!expand)
            scale = scale * scaleStep;
        else
            scale = scale / scaleStep;
    }

    if(rotateset)
    {
        angle = angle + angleStep;
    }

}

void GLWidget::resizeGL(int w, int h)
{
    // initialize viewing values; setup unit view cube
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w/2.0, w/2.0, -h/2.0, h/2.0, -3, 3);
    int side = qMin(w, h);
    glViewport((w - side) / 2, (h - side) / 2, side, side);

}



void GLWidget::setTiles(vector<Tile> &tiles)
{
    m_tiles = tiles;
    //setTimer();
}


void GLWidget::drawTiles()
{

    // error checking
    if (m_tiles.empty()) return;

    // for each tile, get color and draw polygon
    int n_tiles = m_tiles.size();
    for (int i = 0; i<n_tiles; ++i) {


        QColor color = m_tiles[i].color();
        QVector2D ctd = m_tiles[i].centroid();	// set centroid to vtx

             if (centroidset)
             {
                 glPointSize(6);
                 glColor3f(1.0,1.0,1.0);                // white color for centroids
                 glBegin(GL_POINTS);                    // set points mode
                 glVertex3f(ctd.x(), ctd.y(),-1);       // next centroid point
                 glEnd();                               // end polygon mode
             }


        glPushMatrix();
        glTranslatef( ctd.x(),ctd.y() ,0);
        glScalef(scale,scale, scale);
        glRotatef(angle,0,0,1);
        glTranslatef(-ctd.x() , -ctd.y() , 0);

        // color tile
        glColor3f(color.redF(), color.greenF(), color.blueF());
        // draw tile
        glBegin(GL_POLYGON);                        // set polygon mode
        int n_vtx = m_tiles[i].num();               // get number of tile vertices
        for(int j = 0; j<n_vtx; ++j) {              // visit each tile vertex
            QVector2D vtx = m_tiles[i].vertex(j);	// assign (x,y) coords to vtx
            glVertex2f(vtx.x(), vtx.y());       	// assign vtx as next polygon vertex
        }
        glEnd();
     glPopMatrix();
    }
}

