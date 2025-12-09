#include "QtWidgetsApplication1.h"
#include "K8055Adapter.h"
#include <bdd.h>
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

bool tempLoop(QtWidgetsApplication1* window)
{
	K8055Adapter* carteES = K8055Adapter::getInstance();
	carteES->OpenDevice(0);
	BDD b;

	if (carteES->OpenDevice(0)!=-1)
	{
		a = carteES->ReadAnalogChannel(0);
		a = a / 255 * 5;
		c = a * 18 - 30;

		window->ui.label->setText("Temperature : "+ (QString::number(c) +" °C" ));
		b.insertData(static_cast<float>(c));
		return true;
	}
	else {
		window->ui.label->setText("Carte non connectée");
		return false;
	}
}
