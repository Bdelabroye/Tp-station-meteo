#include "QtWidgetsApplication1.h"
#include "K8055Adapter.h"
#include <iostream>
#include <thread>
#include <chrono>

QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{}

float a;
float c;

void tempLoop(QtWidgetsApplication1* window)
{
	K8055Adapter* carteES = K8055Adapter::getInstance();
	carteES->OpenDevice(0);
	// Opérations sur la carte (lectures / écritures)

		a = carteES->ReadAnalogChannel(0);
		a = a / 255 * 5;
		c = a * 18 - 30;
		
		window->ui.label_2->setText(QString::number(c) + " C");
}
