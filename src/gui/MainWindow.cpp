#include "MainWindow.h"

#include <QtWidgets/qlayout.h>

MainWindow::MainWindow() : QMainWindow()
{
	setMouseTracking(true);

	m_pOrbitCamera = std::make_shared<OrbitCamera>();
	m_pCameraController = std::make_shared<CameraController>(m_pOrbitCamera);

	view3D = new View3D(this, m_pOrbitCamera);
	view3D->setMouseTracking(true);
	view3D->setGeometry(200, 0, 500, 500);

	auto mouseEventListener = std::dynamic_pointer_cast<IMouseEventListener>(m_pCameraController);
	view3D->AddEventListener(mouseEventListener);
	
}

MainWindow::~MainWindow()
{
	delete view3D;
}



