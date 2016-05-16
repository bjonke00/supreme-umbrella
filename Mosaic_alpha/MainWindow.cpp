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
	// create load action and set signal-slot connection
	QAction *load = new QAction("&Load Tiles", this);
	load->setShortcut(tr("Ctrl+L"));
	connect(load, SIGNAL(triggered()), this, SLOT(s_loadTiles()));

	// create quit action and set signal-slot connection
	QAction *quit = new QAction("&Quit", this);
	quit->setShortcut(tr("Ctrl+Q"));
	connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));

	// create menus
	QMenu *file;
	file = menuBar()->addMenu("&File");
	file->addAction(load);
	file->addAction(quit);

    // create widget
    m_glwidget = new GLWidget;	// widget for OpenGL drawing

    checkbox_centroid = createcentroidCheckbox();
    checkbox_scale = createscaleCheckbox();
    checkbox_rotate = createrotateCheckbox();
    button_load = new QPushButton("LOAD");
    button_play = new QPushButton("PLAY/PAUSE");

    connect(checkbox_centroid, SIGNAL(stateChanged(int)),m_glwidget, SLOT(setCentroid()));

    connect(checkbox_scale, SIGNAL(stateChanged(int)),m_glwidget, SLOT(setScale()));

    connect(checkbox_rotate, SIGNAL(stateChanged(int)),m_glwidget, SLOT(setRotate()));

    connect(button_load, SIGNAL(clicked()),this, SLOT(s_loadTiles()));

    connect(button_play, SIGNAL(clicked()),m_glwidget, SLOT(setPlay()));

    centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    QGridLayout *centralLayout = new QGridLayout(centralWidget);
    centralLayout->addWidget(m_glwidget,0,1,8,8);
    centralLayout->addWidget(createGroupBox(),0,0);
    centralLayout->setColumnStretch(1,2);
    centralWidget->setLayout(centralLayout);
}

QCheckBox *MainWindow::createcentroidCheckbox()
{
    QCheckBox *checkbox_centroid = new QCheckBox("Show Centroid", this);
    checkbox_centroid->setChecked(false);

    return checkbox_centroid;
}

QCheckBox *MainWindow::createscaleCheckbox()
{
    QCheckBox *checkbox_scale = new QCheckBox("Scale", this);
    checkbox_scale->setChecked(false);

    return checkbox_scale;
}

QCheckBox *MainWindow::createrotateCheckbox()
{
    QCheckBox *checkbox_rotate = new QCheckBox("Rotate", this);
    checkbox_rotate->setChecked(false);

    return checkbox_rotate;
}

QGroupBox *MainWindow::createGroupBox()
{
    QGroupBox *groupbox = new QGroupBox();

    QLabel *label1 = new QLabel("Speed Slider");

    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange(1, 8);
    slider->setValue(1);

    connect(slider, SIGNAL(valueChanged(int)), m_glwidget, SLOT(setSpeed(int)) );
    connect(slider, SIGNAL(valueChanged(int)), m_glwidget, SLOT(setSpeed(int)) );


    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(button_load,0,0);
    mainLayout->addWidget(button_play,0,1);
    mainLayout->addWidget(checkbox_scale,1,0);
    mainLayout->addWidget(checkbox_rotate,2,0);
    mainLayout->addWidget(checkbox_centroid,3,0);
    mainLayout->addWidget(slider,4,0);
    mainLayout->addWidget(label1,4,1);
    mainLayout->setRowStretch(5,3);
    groupbox->setLayout(mainLayout);

    return groupbox;
}


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

			QVector2D vtx((x-w2)/w2, (y-h2)/h2);

			// add vertex to list of vertices in tile object
			tile.addVertex(vtx);
		}

        // set randomn color
		tile.setRandColor();

		// add tile to the m_tiles array
		m_tiles.push_back(tile);
	}

	// close file
	file.close();

	// assign m_tiles to the OpenGL widget 
	m_glwidget->setTiles(m_tiles);
}

