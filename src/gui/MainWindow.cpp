#include "MainWindow.h"

#include <QtWidgets/qlayout.h>

MainWindow::MainWindow() : QMainWindow()
{
	view3D = new View3D();
	setCentralWidget(view3D);
}

MainWindow::~MainWindow()
{
	delete view3D;
}
