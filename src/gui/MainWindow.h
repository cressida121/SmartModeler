#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtCore/QTimer>
#include <glm/gtc/matrix_transform.hpp>

#include <gui/view3d/View3D.h>

// Qt code is allowed only inside classes that inherit from QObject
class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

private:
	View3D* view3D = nullptr;
};

#endif // MAIN_WINDOW_H