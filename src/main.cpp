#include <QtWidgets/QApplication.h>

#include <gui/MainWindow.h>


int main()
{
	QApplication app(__argc, __argv);

	MainWindow mainWindow;
	mainWindow.resize(700, 500);
	
	mainWindow.show();

	return app.exec();

}