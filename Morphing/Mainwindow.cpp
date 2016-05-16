#include "Mainwindow.h"

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
    m_LoadButton = new QPushButton("Load");
    m_playPause  = new QPushButton("Play/Pause");

    // make drawing widget
    m_glwidget = new GLWidget;  // widget for OpenGL drawing

    // make slider
    m_slider = new QSlider(Qt::Horizontal);
    m_slider->setMinimum(1);
    m_slider->setMaximum(10);
    m_slider->setValue(1);

    // add pushbuttons into horizontal alignment
    QHBoxLayout *buttons = new QHBoxLayout;
    buttons->addStretch();
    buttons->addWidget(m_LoadButton);
    buttons->addWidget(m_playPause);
    buttons->addStretch();

    // add slider
    QLabel *speed = new QLabel("Speed");
    QHBoxLayout *slider = new QHBoxLayout;
    slider->addStretch();
    slider->addWidget(speed);
    slider->addWidget(m_slider);
    slider->addStretch();

    // add main layout
    QVBoxLayout *settingLayout = new QVBoxLayout;
    settingLayout->addLayout(buttons);
    settingLayout->addLayout(slider);
    settingLayout->addStretch();


    // add settings and OpenGL widget into horizontal alignment
    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addLayout(settingLayout, 1, 0);//row, column
    mainLayout->addWidget(m_glwidget,    0, 0);//row, column
    mainLayout->setRowStretch(0, 1);           //row, amount to stretch(also setColumnStretch for columns)

    //5.Setting Main Layout
    QWidget *window = new QWidget;
    window ->setLayout(mainLayout);
    setCentralWidget(window);

    // set signal-slot connections
    connect(m_LoadButton,SIGNAL(clicked()),this, SLOT(s_loadTiles()));
    connect(m_playPause, SIGNAL(clicked()), m_glwidget, SLOT(s_play()));
    connect(m_slider,  SIGNAL(valueChanged(int)), m_glwidget, SLOT(s_setSpeed(int)));

}


//Slot Functions:

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// loadTiles:
// 
// Prompts the user to pick the files for the tiles. We then load
// the tiles into our GLWidget as well as load the textures. lastly
// we set the timer for our widget.
//

void MainWindow::s_loadTiles ()
{
    QString fileName  = QFileDialog::getOpenFileName(this, "Open Tiles", "", "Tiles (*.txt)");

    // error checking
    if (fileName == NULL) return;

    // assign m_tiles to the OpenGL widget
    m_glwidget->loadTiles(fileName,0);

    QString fileName2  = QFileDialog::getOpenFileName(this, "Open Tiles", "", "Tiles (*.txt)");

    // error checking
    if (fileName2 == NULL) return;

    m_glwidget->loadTiles(fileName2,1);

    // Once both tile is loaded find initial and final for morph
    m_glwidget->getMorph();


    m_glwidget->setTimer();
}
