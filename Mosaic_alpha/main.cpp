#include "MainWindow.h"
#include <QtWidgets>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main.cpp - main() function.
//

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);		// create application

	MainWindow	mainwin;		// create main window
    mainwin.resize(1000,750);		// init default size
	mainwin.show();
	//mainwin.showMaximized();		// display window
	return app.exec();			// infinite processing loop
}
