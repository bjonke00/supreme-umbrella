#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtGui>
#include <QtOpenGL>
#include <GL/glu.h>
#include "Tile.h"
#include "shapemorph.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

    public:
         GLWidget();
        ~GLWidget();

        void loadTiles	     (QString &, int);
        void drawTiles       ();
        void setTimer        ();
        void updateTiles     ();

        void radialMotion    (Tile &);
        void radialMotion2   (Tile &);
        void loadTexture     ();
        void loadTexture2    ();

        void getMorph        ();


    public slots:
        void s_play                 ();
        void s_setSpeed             (int);
        void s_setCentroid          (int);
        void s_setScale             (int);
        void s_setRotate            (int);

    protected:
        void initializeGL   ();
        void paintGL        ();
        void resizeGL       (int, int);

    private:
        vector<Tile>    m_tiles;
        vector<Tile>    m_tiles2;
        QTimer*         m_Timer;
        bool            m_play;
        bool            m_flagCentroid;
        bool            m_flagRotate;
        bool            m_flagScale;

        QString         m_imgFileName;
        QString         m_imgFileName2;

        float           m_speed;
        float           m_scale;

        int             m_state;
        Tile            m_morphTile;
        shapemorph      m_shapemorph;
        float           m_t;

};

#endif  // GLWIDGET_H
