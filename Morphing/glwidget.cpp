#include "glwidget.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Class Constructor
//

GLWidget::GLWidget()
{
    m_speed           = 1.0f;   // Set angle multiplier
    m_scale           = 1.0;    // Set Scale factor
    m_t = 0.0;
    m_state = 0;
    m_play            = false;  // Set play flag
    m_flagCentroid    = false;  // Set centroid flag
    m_flagRotate      = false;  // Set rotate flag
    m_flagScale       = false;  // Set scale flag
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Class Destructor
//

GLWidget::~GLWidget()
{}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// InitializeGL:
// 
// Initializes our GLWidget by enabling smooth point, set background,
// enable depth test, and enable lighting
//

void GLWidget::initializeGL()
{
    qglClearColor(QColor(0., 0., 0., 1.0));

    // Enable smooth points
    glEnable(GL_POINT_SMOOTH);

    // Enable depth test
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);               // Set background color to black and opaque
    glClearDepth(1.0f);                                 // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);                            // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);                             // Set the type of depth-test
    glShadeModel(GL_SMOOTH);                            // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);                              // Enable lighting
    glEnable(GL_LIGHT0);                                // Enable light #0
    glEnable(GL_LIGHT1);                                // Enable light #1
    glEnable(GL_NORMALIZE);                             // Automatically normalize normals
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// setTimer:
//
// Initializes our timer for 1 frame per 10ms. Once 10ms past then
// we call updateGL to update our widget.
//

void GLWidget::setTimer()
{
    // Set Timer to 10 ms
    m_Timer = new QTimer(this);
    m_Timer->start(10);

    // Set conncetion for timer
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(updateGL()));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// resizeGL:
// 
// Sets the size of our GLWidget.
//

void GLWidget::resizeGL(int w, int h)
{
    int side = qMin(w, h);
    glViewport((w - side) / 2, (h - side) / 2, side, side);

    // initialize viewing values; setup unit view cube
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1.0, .01, 1000.);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// paintGL:
//
// This updates our animation states such as tile angle and size.
// We also set light color, light position, and camera posistion.
// Also it call our render in this function to see our animation.
//

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
    glTranslatef(0, 0, 3.0);

    gluLookAt(0.0, 0.0, 4.0, 0, 0, 0, 0.0, 1.0, 0.0);

    // Render our tiles
    drawTiles();

    // Update our tiles for next frame
    updateTiles();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// drawTiles:
// 
// This is our renderer where we draw and animates our tiles. It
// first renders the first set of tiles and then the second set.
//

void GLWidget::drawTiles()
{

    if(m_morphTile.num() == 0) return;
    //if(m_tiles.empty()) return;

    glColor3f(0.0,1.0,0.5);

    glBegin(GL_POLYGON);                                    // set polygon mode
    int n_vtx = m_morphTile.num();                           // get number of tile vertices
    for(int j = 0; j<n_vtx; ++j) {                          // visit each tile vertex
        QVector2D vtx = m_morphTile.vertex(j);
        glVertex3f(vtx.x(), vtx.y(), 0.0);   // assign vtx as next polygon vertex
    }
    glEnd();                                                // end polygon mode

}

void GLWidget::updateTiles()
{

    switch(m_state) {
        case 0:
        {
            m_t += 0.005f * m_speed;
            if(m_t >= 1.0) {
                m_t = 1.0;
                m_state = 1;
            }
        } break;

        case 1:
        {
            m_t -= 0.005f * m_speed;
            if(m_t <= 0.0) {
                m_t = 0.0;
                m_state = 0;
            }
        } break;
    }

    Tile tempTile;
    m_shapemorph.InterPolate(m_t, tempTile);
    m_morphTile = tempTile;

}

void GLWidget::getMorph()
{
    m_shapemorph.FindSourceDest(m_tiles[0], m_tiles2[0]);
    m_shapemorph.InterPolate(0, m_morphTile);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// loadTiles:
//
// Loads our tiles so we are able to render them later on. We also
// get the name of the our texture file for texture mapping.
//

void GLWidget::loadTiles(QString &fileName, int flag)
{

        QFileInfo *info = new QFileInfo(fileName);

        if(flag == 0)
            m_imgFileName = info->path() + "/" + info->baseName() + ".jpg";
        if(flag == 1)
            m_imgFileName2 = info->path() + "/" + info->baseName() + ".jpg";

        // open file for reading a stream of text
        QFile   file(fileName);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);

        // read first three lines to get mosaic width, height, and number of tiles;
        // start with first line: mosaic width
        QString line = in.readLine();       // read mosaic width (inches)
        int   w  = line.toInt();            // convert string to int
        float w2 = w >> 1;                  // half-width for computing center

        // read second line: mosaic height
        line = in.readLine();               // read mosaic height (inches)
        int h  = line.toInt();              // convert string to int
        int h2 = h >> 1;                    // half-height for computing center

        // read third line: number of tiles
        line = in.readLine();               // read number of tiles
        int n_tiles = line.toInt();         // convert string to int
        for (int i = 0; i<n_tiles; ++i) {
            Tile tile;
            line = in.readLine();           // read number of vertices
            int n_vertices = line.toInt();  // convert string to int
            tile.setNum(n_vertices);        // set it in tile class

            // visit all vertices
            for (int j = 0; j<n_vertices; ++j) {
                line = in.readLine();       // read coordinates
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
            if(flag == 0) {
                tile.setDepth(0);
                m_tiles.push_back(tile);
            }
            if(flag == 1) {
                tile.setDepth(2.5);
                m_tiles2.push_back(tile);
            }
        }

        // close file
        file.close();

        //Play Button Set to Enabled Position
        m_play = true;
}

//Slot Functions:

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// s_play:
//
// Stop or plays our animation by stopping or starting our timer.
//

void GLWidget::s_play()
{
    m_play =!m_play;
    if (m_play) m_Timer->start();
    else        m_Timer->stop();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// s_setSpeedMultiplier:
//
// Sets our multiplier for our speed of animation.
//

void GLWidget::s_setSpeed(int spinspeed)
{
    m_speed = spinspeed;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// s_setcentroid:
//
// Sets our centroid flag to decide when to show our centroid
//

void GLWidget::s_setCentroid(int flag)
{
    m_flagCentroid = flag;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// s_setScale
//
// Sets our scaling flag to decide when to scale our tiles.
//

void GLWidget::s_setScale(int flag)
{
    m_flagScale = flag;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// s_setRotate:
//
// Sets our rotating flag to decide when to rotate our tiles.
//

void GLWidget::s_setRotate(int flag)
{
    m_flagRotate = flag;
}



