
// ======================================================================
//
// MainWindow.h - Main window widget class
//
// ======================================================================

#include "MainWindow.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::MainWindow:
//
// Constructor. Initialize user-interface elements.
//
MainWindow::MainWindow(QWidget *parent)
      : QMainWindow(parent)
{
    // create GUI with actions, menus, widgets, and layouts
    createActions();	// create actions for each menu item
    createMenus  ();	// create menus and associate actions
    createWidgets();	// create window widgets
}


//! \brief createActions - creates actions and shortcuts.
/*!       Create actions and shortcuts for mainwindow menu and toolbar.
 *        Load Tiles action is created along with shortcut 'ctrl+l'.
 *        Quit action is created along with shortcut 'ctrl-q'.
 *
 *
 * */
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createActions:
//
// Create actions to associate with menu and toolbar selection.
//
void
MainWindow::createActions()
{
    // create load action and set signal-slot connection
    m_actionLoad = new QAction("&Load Tiles", this);
    m_actionLoad->setShortcut(tr("Ctrl+L"));
    connect(m_actionLoad, SIGNAL(triggered()), this, SLOT(s_loadTiles()));

    // create quit action and set signal-slot connection
    m_actionQuit = new QAction("&Quit", this);
    m_actionQuit->setShortcut(tr("Ctrl+Q"));
    connect(m_actionQuit, SIGNAL(triggered()), this, SLOT(close()));
}


//! \brief  Create Menus
/*!         fileMenu is created and contains actions load and quit.
 * */
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createMenus:
//
// Create menus and install in menubar.
//
void
MainWindow::createMenus()
{
    QMenu *fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(m_actionLoad);
    fileMenu->addAction(m_actionQuit);
}


//! \brief  Create Widgets
/*!       Setup the layout using grids.  Connect the signals and slots.
 *
 *
 * */
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createWidgets:
//
// Create widgets to put in the layout.
//
void
MainWindow::createWidgets()
{
    // make load and play/pause pushbuttons
    m_load	   = new QPushButton("Load");
    m_play	   = new QPushButton("Play/Pause");
    m_reset    = new QPushButton("Reset");
    m_spiral   = new QCheckBox("Spiral Effect", this);
    m_space    = new QCheckBox("Space Effect", this);

    //mouse
    xSlider = createSlider();
    ySlider = createSlider();
    zSlider = createSlider();
    //zoomSlider = createSlider();

    // make drawing widget
    m_glwidget = new GLWidget;	// widget for OpenGL drawing
    setCentralWidget(m_glwidget);	// set it to be central widget

    // set signal-slot connections
    connect(m_load,     SIGNAL(clicked()),		this,	    SLOT(s_loadTiles()));
    connect(m_play,     SIGNAL(clicked()),		m_glwidget, SLOT(s_play()));
    connect(m_reset,     SIGNAL(clicked()),		m_glwidget, SLOT(s_reset()));
    connect(m_spiral,   SIGNAL(stateChanged(int)),	m_glwidget, SLOT(s_spiral()));
    connect(m_space,     SIGNAL(stateChanged(int)),	m_glwidget, SLOT(s_space()));

    //mouse signals and slots both ways
    connect(xSlider, SIGNAL(valueChanged(int)), m_glwidget, SLOT(setXRotation(int)));
    connect(m_glwidget, SIGNAL(xRotationChanged(int)), xSlider, SLOT(setValue(int)));
    connect(ySlider, SIGNAL(valueChanged(int)), m_glwidget, SLOT(setYRotation(int)));
    connect(m_glwidget, SIGNAL(yRotationChanged(int)), ySlider, SLOT(setValue(int)));
    connect(zSlider, SIGNAL(valueChanged(int)), m_glwidget, SLOT(setZRotation(int)));
    connect(m_glwidget, SIGNAL(zRotationChanged(int)), zSlider, SLOT(setValue(int)));

    //set initial checks
    m_spiral->setChecked(false);
    m_space->setChecked(false);

    // add pushbuttons into horizontal alignment
    QHBoxLayout *buttons = new QHBoxLayout();
    buttons->addStretch();
    buttons->addWidget(m_load);
    buttons->addWidget(m_play);
    buttons->addWidget(m_reset);
    buttons->addStretch();

    // add checkboxes into vertical alignment
    QVBoxLayout *checkboxes = new QVBoxLayout();
    checkboxes->addWidget(m_spiral);
    checkboxes->addWidget(m_space);
    checkboxes->addStretch();

    // add widgets for settings into vertical alignment
    QHBoxLayout *settingLayout = new QHBoxLayout();
    settingLayout->addLayout(buttons);
    settingLayout->addLayout(checkboxes);
    //settingLayout->addStretch();

    // add settings and OpenGL widget into horizontal alignment
    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addLayout(settingLayout, 1, 0);//row, column
    mainLayout->addWidget(m_glwidget,    0, 0);//row, column
    mainLayout->setRowStretch(0, 1);           //row, amount to stretch(also setColumnStretch for columns)

    // make the OpenGL widget be the central widget
    QWidget *central = new QWidget();
    central->setLayout(mainLayout);
    setCentralWidget(central);
}

QSlider *MainWindow::createSlider()
{
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange(0, 360);
    return slider;
}


//! \brief
/*!       Load tile geometry.
 *        Slot function for file/load.
 *        Loads texture as well.
 *
 * */
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::s_loadTiles:
//
// Slot function for File|Load.
// Loads tile geometry.
//
void MainWindow::s_loadTiles()
{
    // launch file dialog and get file containing tile geometry
    QString fileName = QFileDialog::getOpenFileName(this, "Open Tiles", "", "Tiles (*.txt)");

    // error checking
    if (fileName == NULL) return;


    // assign m_tiles to the OpenGL widget
    m_glwidget->loadTiles(fileName);
    m_glwidget->loadTexture();
    m_glwidget->setTimer();
}

