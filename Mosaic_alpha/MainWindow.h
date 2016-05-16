// ======================================================================
//
// MainWindow.h - Main window widget class
//
// ======================================================================

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "Tile.h"
#include "GLWidget.h"
#include <QGroupBox>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>

// forward declaration
class GLWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	// constructor
	MainWindow(QWidget *parent = 0);

public slots:
	// slots
	void s_loadTiles();		// read tile geometry from file

private:
	vector<Tile>	 m_tiles;	// array of mosaic tiles
	GLWidget	*m_glwidget;	// widget in which to draw
    QGroupBox *createGroupBox();
    QWidget *centralWidget;
    QScrollArea *myglWidgetArea;
    QPushButton *button_load;
    QPushButton *button_play;
    QCheckBox *createcentroidCheckbox();
    QCheckBox *checkbox_centroid;
    QCheckBox *createscaleCheckbox();
    QCheckBox *checkbox_scale;
    QCheckBox *createrotateCheckbox();
    QCheckBox *checkbox_rotate;

};

#endif	// MAINWINDOW_H
