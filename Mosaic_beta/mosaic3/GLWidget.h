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
#include "Tile.h"
#include <GL/glu.h>

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
     GLWidget(QWidget *parent = 0);
    ~GLWidget();

    void		setTiles	(vector<Tile> &);
    void		drawTiles	();
    void		setTimer	();
    void		radialMotion	(Tile &);
    QVector3D	computeNormal	(QVector2D &, QVector2D &, float, float);
    void		loadTiles	(QString &);
    void		loadTexture();
    void		loadTexture1();

protected:
    void	initializeGL	();
    void	paintGL		();
    void	resizeGL	(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void	s_setScale	(int flag);
    void	s_setRotate	(int flag);
    void	s_setCentroid	(int flag);
    void	s_setSpeed	(int speed);
    void	s_play	   ();
    void    s_spiral   ();
    void    s_space    ();
    void    s_reset    ();

    //mouse
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    //void setZoom(int amount);

signals:
    //mouse
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);


private:
    QString		m_imageFileName;
    vector<Tile>	m_tiles;
    QTimer		*m_timer;
    float		m_angle;
    float		m_scale;
    float		m_angleStep;
    float		m_scaleStep;
    float		m_r2;
    bool		m_expand;
    bool		m_play;
    bool		m_flagScale;
    bool		m_flagRotate;
    bool		m_flagCentroid;
    float		m_globaleyeX;
    float		m_globaleyeY;
    float		m_globaleyeZ;
    GLuint		m_texture;
    GLuint		m_texture1;
    bool		m_scaleDone;
    float       m_thickness;
    bool        m_spiral;
    bool        m_space;
    bool        m_reset;
    //mouse
    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;

};

#endif	// GLWIDGET_H
