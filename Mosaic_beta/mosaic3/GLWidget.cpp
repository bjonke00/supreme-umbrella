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

    // Initial Rotation Angle | Scaling Factor | Step Amounts | Thickness (Tiles)
    m_angle         = 0.0f;
    m_scale         = 1.0f;
    m_scaleStep     = 0.995f;
    m_angleStep     = 1.0f;
    m_thickness     = 0.01f;

    // Initial Settings | Flags (Initialize to False)
    m_scaleDone     = false;
    m_expand        = false;
    m_play          = false;
    m_flagScale     = false;
    m_flagRotate    = false;
    m_flagCentroid  = false;

    // Extra Settings | Flags
    m_spiral = false;
    m_space  = false;
    m_reset  = false;

    // Initial Radial Explosion
    m_r2 = 0.0f;

    // Initial Mouse Settings
    xRot = 0;
    yRot = 0;
    zRot = 0;
}



GLWidget::~GLWidget()
{}

// Timer Set to Update Rendering
void GLWidget::setTimer()
{
    m_timer = new QTimer(this);
    m_timer->start(5);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
}

// Mouse Rotation
static void qNormalizeAngle(int &angle)
 {
     while (angle < 0)
         angle += 360;
     while (angle > 360)
         angle -= 360;
 }

void GLWidget::setXRotation(int angle)
 {
     qNormalizeAngle(angle);
     if (angle != xRot)
     {
         xRot = angle;
         emit xRotationChanged(angle);
         updateGL();
     }
 }

void GLWidget::setYRotation(int angle)
  {
      qNormalizeAngle(angle);
      if (angle != yRot)
      {
          yRot = angle;
          emit yRotationChanged(angle);
          updateGL();
      }
  }

void GLWidget::setZRotation(int angle)
  {
      qNormalizeAngle(angle);
      if (angle != zRot)
      {
          zRot = angle;
          emit zRotationChanged(angle);
          updateGL();
      }
  }


void GLWidget::initializeGL()
{
    qglClearColor(QColor(0., 0., 0., 1.0));

    // enable smooth points
    glEnable(GL_POINT_SMOOTH);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Set background color to black and opaque
    glClearDepth(1.0f);                     // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);                // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);                 // Set the type of depth-test
    glShadeModel(GL_SMOOTH);                // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);                  //Enable lighting
    glEnable(GL_LIGHT0);                    //Enable light #0
    glEnable(GL_LIGHT1);                    //Enable light #1
    glEnable(GL_NORMALIZE);                 //Automatically normalize normals
}


// Refresh widget by drawing the tiles and modifying the scale/rotate parameters.
void GLWidget::paintGL()
{
    // clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // add ambient light
    GLfloat ambientColor[] = {0.5f, 0.5f, 0.5f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    // add positioned light
    GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat lightPos0  [] = {4.0f, 0.0f, 8.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    // add directed light
    GLfloat lightColor1[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat lightPos1  [] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

    // update modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // move camera and look at origin
    // Translate to XYZ Coordinate Systems
    glTranslatef(0, 0, 3.0);

    // Creates a viewing matrix derived from an eye point.
    //EyeXYZ | CenterXYZ | UpXYZ
    gluLookAt(0, -2, 5, 0, 0, 0, 0.0, 0.0, 1.0);

    //Draw the tiles
    drawTiles();

    // update scale in back-and-forth manner
    // Scaling down individual tiles
        if ( m_scale < .5 ){        // Scale limit 0.5
            m_expand    = false;    // Stop Expanding
            m_scaleDone = true;     // Scaling Done
        }
        else{
            m_expand    = true;
        }

        // Update Angle
        if ( m_expand ){
            m_scale *= m_scaleStep;
            }

        // Explosion Radius
        if (m_scaleDone){
            m_r2 += 0.0005f; //Scale Factor
            }

}


void GLWidget::resizeGL(int w, int h)
{
    int side = qMin(w, h);
    glViewport((w - side) / 2, (h - side) / 2, side, side);

    // initialize viewing values; setup unit view cube
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1.0, 0.01, 1000.0);//field of view angle, aspect ratio, z near, z far

}

// Copy a vector (array) of tiles and set the animation timer.
void GLWidget::setTiles(vector<Tile> &tiles)
{
    m_tiles = tiles;
    setTimer();
}

//! DrawTiles rotating/scaled tile animation.
/*! Draw the tiles starting with glBegin using GL_POLYGONS to draw rectangular prism tiles.
 *  Then glVertex3f--for xyz floats (3dimensional).
 *  Next the texture is implemented and close with  glEnd.
 *  Implements when m_spiral is set true by checkbox.
 *  Implements spacing effect when m_space is set true by checkbox.
 *  Calls radial motion effect in order to explode implode.
 *  Within the glPushMatrix() and glPopMatrix() there is a translate, rotate, scale, and then translate back.
 *  This is to rotate and scale the tiles about their centroids.
 **/
// Draw rotating/scaled tile animation.
void GLWidget::drawTiles()
{
    //error checking
    if (m_tiles.empty()) return;

    //set number of tiles
    size_t n_tiles = m_tiles.size();
    for (size_t i = 0; i<n_tiles; ++i)               // visit each tile
    {
        radialMotion(m_tiles[i]);                    //move tile if within radius:
        QVector2D cen = m_tiles[i].centroid();       //cen is centroid of ith tile
        glPushMatrix();                              //push state

            if(m_spiral){   //if true spiral effect
            if(m_space)     //if true spacing
            {
                glTranslatef(cen.x(), cen.y(), m_tiles[i].depth());
                glScalef(m_scale, m_scale, m_scale);
                glTranslatef(-cen.x(), -cen.y(), -m_tiles[i].depth());
            }
            glRotatef(m_tiles[i].angleX(),1,0,0);
            glRotatef(m_tiles[i].angleY(),0,1,0);
            glRotatef(m_tiles[i].angleZ(),0,0,1);

            }
            else
            {
                //Initial scaling of individual tiles
                glTranslatef(cen.x(), cen.y(), m_tiles[i].depth());
                if(m_space)
                glScalef(m_scale, m_scale, m_scale);
                glRotatef(m_tiles[i].angleX(),1,0,0);
                glRotatef(m_tiles[i].angleY(),0,1,0);
                glRotatef(m_tiles[i].angleZ(),0,0,1);
                glTranslatef(-cen.x(), -cen.y(), -m_tiles[i].depth());
            }
            // Enable Texture Mapping
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, m_texture);                            // Pixels in the Texture are addressed using Texture Coordinates.

            // Begin Drawing Polygon
            glBegin(GL_POLYGON);                                                // set polygon mode
            int n_vtx = m_tiles[i].num();                                       // get number of tile vertices

            for(int j = 0; j<n_vtx; ++j)                                        // visit each tile vertex
            {
                QVector2D vtx = m_tiles[i].vertex(j);                           // assign (x,y) coords to vtx
                glTexCoord2f(vtx.x() + 0.5, vtx.y() + 0.5);                     // texture coordinates between 0 and 1
                glVertex3f(vtx.x(), vtx.y(), m_tiles[i].depth()+ m_thickness);	// assign 3d vertex

            }
            glEnd();                                                            //end drawing polygon

            for(int j = 0; j<n_vtx-1; ++j)
            {
                QVector2D vtx1 = m_tiles[i].vertex(j);
                QVector2D vtx2 = m_tiles[i].vertex(j+1);

                glBegin(GL_POLYGON);
                    glTexCoord2f(vtx1.x() + 0.5, vtx1.y() + 0.5);
                    glVertex3f(vtx1.x(), vtx1.y(), m_tiles[i].depth());
                    glTexCoord2f(vtx2.x() + 0.5, vtx2.y() + 0.5);
                    glVertex3f(vtx2.x(), vtx2.y(), m_tiles[i].depth());
                    glTexCoord2f(vtx2.x() + 0.5, vtx2.y() + 0.5);
                    glVertex3f(vtx2.x(), vtx2.y(), m_tiles[i].depth() + m_thickness);
                    glTexCoord2f(vtx1.x() + 0.5, vtx1.y() + 0.5);
                    glVertex3f(vtx1.x(), vtx1.y(), m_tiles[i].depth() + m_thickness);
                glEnd();
            }

            QVector2D vtx1 = m_tiles[i].vertex(n_vtx-1);
            QVector2D vtx2 = m_tiles[i].vertex(0);

            glBegin(GL_POLYGON);
                glTexCoord2f(vtx1.x() + 0.5, vtx1.y() + 0.5);
                glVertex3f(vtx1.x(), vtx1.y(), m_tiles[i].depth());
                glTexCoord2f(vtx2.x() + 0.5, vtx2.y() + 0.5);
                glVertex3f(vtx2.x(), vtx2.y(), m_tiles[i].depth());
                glTexCoord2f(vtx2.x() + 0.5, vtx2.y() + 0.5);
                glVertex3f(vtx2.x(), vtx2.y(), m_tiles[i].depth() + m_thickness);
                glTexCoord2f(vtx1.x() + 0.5, vtx1.y() + 0.5);
                glVertex3f(vtx1.x(), vtx1.y(), m_tiles[i].depth() + m_thickness);
            glEnd();

            glDisable(GL_TEXTURE_2D);
                                                                        //draw the tile
            glBegin(GL_POLYGON);                                        //set polygon mode
                for(int j = 0; j<n_vtx; ++j)                            //visit each tile vertex
                {
                    QVector2D vtx = m_tiles[i].vertex(j);               //assign (x,y) coords to vtx
                    glTexCoord2f(vtx.x() + 0.5, vtx.y() + 0.5);         //set texture
                    glVertex3f(vtx.x(), vtx.y(), m_tiles[i].depth());	//assign 3d vertex
                }
            glEnd();                                                    //end drawing tile

            glPopMatrix();                                              //pop state

    }


}

QVector3D GLWidget::computeNormal(QVector2D &vtx1, QVector2D &vtx2, float depth, float thickness)
{
    QVector3D p1(vtx1.x(), vtx1.y(), depth);
    QVector3D p2(vtx2.x(), vtx2.y(), depth);
    QVector3D p3(vtx1.x(), vtx1.y(), depth+thickness);
    QVector3D v1 = p2 - p1;
    QVector3D v2 = p3 - p1;
    QVector3D v3 = QVector3D::crossProduct(v1, v2);

    return v3.normalized();
}

//! \brief Update the depth and angles of tiles
/*!         The distance 'dist2' is set to x^2+y^2 of the centroids, which represents paraboloid.
 *          While the distance is less than the radius 'm_r2' and the radius is less than 0.6 (sweet spot)
 *          the depth and angles increase by random fraction.  This gives Explosion effect.  Once the tiles reach the sweet spot
 *          the depth and angles are decreased to create Implosion effect.
 *
 * */
void GLWidget::radialMotion(Tile &tile)
{
    QVector2D cen = tile.centroid();
    float dist2 = (cen.x()*cen.x() + cen.y()*cen.y());
    //Update depth and angles of tiles within certain radius up to 0.6f
    if(dist2 <= m_r2 && m_r2 <= 0.6f)//0.6f used in order for all tiles to leave screen before falling
    {
        tile.updateDepth((rand()%10000)/500000.0);
        tile.updateAngles((rand()%10)/3.0, (rand()%10)/3.0, (rand()%10)/10.0);
    }

    else  // Implode -- update depth and angles of tiles again until they reach starting point 0.0f
    {
        if (tile.depth() > 0.0f)
            tile.updateDepth(-(rand()%10000)/500000.0);
        if(tile.angleX() != 0 || tile.angleY() != 0 || tile.angleZ() != 0 )
            tile.updateAngles(-tile.angleX(), -tile.angleY(), -tile.angleZ());
    }
}


//! \brief Slot function for setting the scale flag.
// Slot function for setting the scale flag.
void GLWidget::s_setScale(int flag)
{
    m_flagScale = flag;
}

//! \brief Slot function for setting the rotate flag.
// Slot function for setting the rotate flag.
void GLWidget::s_setRotate(int flag)
{
    m_flagRotate = flag;
}

//! \brief Slot function for setting the centroid flag.
// Slot function for setting the centroid flag.
void GLWidget::s_setCentroid(int flag)
{
    m_flagCentroid = flag;
}

//! \brief Slot function for setting the animation speed.
// Slot function for setting the animation speed.
void GLWidget::s_setSpeed(int speed)
{
    m_angleStep = speed;
    m_scaleStep = pow(.995,speed);
}

//! \brief Slot function for playing/pausing the animation.
// Slot function for playing/pausing the animation.
void GLWidget::s_play()
{
    m_play = !m_play;
    //radial = !radial;
    if(m_play)
        m_timer->start();
    else
        m_timer->stop();
}

//! \brief Slot function for spiral effect
// Slot function for spiral effect
void GLWidget::s_spiral()
{
    m_spiral = !m_spiral;

}

//! \brief Slot to control scale space
// Slot to control scale space
void GLWidget::s_space()
{
    m_space = !m_space;

}


//! \brief Reset slot reinitializes initial values.
/*!        Resets all initial values and reinitiates the explosion.
 *
 * */
// Slot reset button
void GLWidget::s_reset()
{
    m_reset = !m_reset;

    // Set all values to default
    m_angle         = 0.0f;
    m_scale         = 1.0f;
    m_scaleStep     = 0.995f;
    m_angleStep     = 1.0f;
    m_scaleDone     = false;
    m_expand        = false;
    m_play          = false;
    m_flagScale     = false;
    m_flagRotate    = false;
    m_flagCentroid  = false;
    m_thickness     = 0.01f;
    m_r2 = 0.0f;

}

void GLWidget::loadTiles(QString &fileName)
{
    QFileInfo *info = new QFileInfo(fileName);
    m_imageFileName = info->path() + "/" + info->baseName() + ".jpg";
    // open file for reading a stream of text
    QFile	file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);

    // read first three lines to get mosaic width, height, and number of tiles;
    // start with first line: mosaic width
    QString line = in.readLine();		// read mosaic width (inches)
    int   w  = line.toInt();		// convert string to int
    float w2 = w >> 1;			// half-width for computing center

    // read second line: mosaic height
    line = in.readLine();			// read mosaic height (inches)
    int h  = line.toInt();			// convert string to int
    int h2 = h >> 1;			// half-height for computing center

    // read third line: number of tiles
    line = in.readLine();			// read number of tiles
    int n_tiles = line.toInt();		// convert string to int
    for (int i = 0; i<n_tiles; ++i) {
        Tile tile;
        line = in.readLine();		// read number of vertices
        int n_vertices = line.toInt();	// convert string to int
        tile.setNum(n_vertices);	// set it in tile class

        // visit all vertices
        for (int j = 0; j<n_vertices; ++j) {
            line = in.readLine();	// read coordinates
            double x = line.section(',', 0, 0).toDouble();
            double y = line.section(',', 1, 1).toDouble();

            // constructor function assigns normalized coordinate [-1,1] to vtx;
            // note: w2 and h2 are floats so that result is float too
            QVector2D vtx((x-w2)/w, (y-h2)/h);
            // add vertex to list of vertices in tile object
            tile.addVertex(vtx);
        }
        // set a random color for tile
        tile.setRandColor();
        tile.setCentroid();

        // add tile to the m_tiles array
        m_tiles.push_back(tile);
    }

    // close file
    file.close();
}

/*!
 * \brief GLWidget::loadTexture
 * glGenTextures(num,&textarray) is used to generate the texture.
 * Arguments are number of textures and the texture array.
 */
void GLWidget::loadTexture()
{

    QImage img(m_imageFileName);

    QImage GL_formatted_image;
    GL_formatted_image = QGLWidget::convertToGLFormat(img);

    glGenTextures(1, &m_texture);               //number of textures, array of texture

    //bind the texture ID
    glBindTexture(GL_TEXTURE_2D, m_texture);

    //texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //generate the texture
    glTexImage2D(   GL_TEXTURE_2D, 0, GL_RGBA, GL_formatted_image.width(),
                    GL_formatted_image.height(),
                    0, GL_RGBA, GL_UNSIGNED_BYTE, GL_formatted_image.bits() );
    glTexEnvf      (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

}

/*! mousePressEvent added
 *  Allows mouse position to update on press.
 **/
//mouse
void GLWidget::mousePressEvent(QMouseEvent *event)
 {
     lastPos = event->pos();
 }

/*! GLWidget mouseMoveEvent
 *  Updates x and y coordinates and allocates left or else right click and movement.
 */
 void GLWidget::mouseMoveEvent(QMouseEvent *event)
 {
     int dx = event->x() - lastPos.x();
     int dy = event->y() - lastPos.y();

     if (event->buttons() & Qt::LeftButton)
          {
              setXRotation(xRot + dy);
              setYRotation(yRot + dx);
          } else if (event->buttons() & Qt::RightButton)
            {
              setXRotation(xRot + dy);
              setZRotation(zRot + dx);
            }
     lastPos = event->pos();
 }
