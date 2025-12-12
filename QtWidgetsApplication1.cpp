#include "QtWidgetsApplication1.h"
#include "K8055Adapter.h"
#include <bdd.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <QGraphicsTextItem>
#include <QTimer>

QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	scene = nullptr;
	connect(ui.Submit, &QPushButton::clicked, this, &QtWidgetsApplication1::getGraphTemperature);
	// Initialiser les dates par défaut
	ui.dateStart->setDateTime(QDateTime(QDate(2000, 1, 1), QTime(0, 0)));
	ui.dateEnd->setDateTime(QDateTime::currentDateTime());

	// Optionnel : lancer le graphique immédiatement
	getGraphTemperature();

	// Actualisation automatique toutes les 5 secondes
	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &QtWidgetsApplication1::getGraphTemperature);
	timer->start(5000); // 5000 ms = 5 secondes
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

void QtWidgetsApplication1::getGraphTemperature()
{
    BDD b;
    QDateTime startDate = ui.dateStart->dateTime();
    QDateTime endDate;
    if (ui.checkBox->isChecked()) {
        endDate = QDateTime::currentDateTime();
        ui.dateEnd->setDateTime(endDate);
    }
    else {
        endDate = ui.dateEnd->dateTime();
    }

    std::vector<std::pair<QDateTime, float>> data = b.getDataBetweenDates(startDate, endDate);

    int width = ui.graphicsView->width();
    int height = ui.graphicsView->height();

    if (scene != nullptr) {
        scene->deleteLater();
    }
    
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, width, height);
    ui.graphicsView->setScene(scene);

    QPen pen(Qt::blue);

    if (data.size() < 2) return;

    // Calcul min/max température
    float minTemp = data[0].second;
    float maxTemp = data[0].second;
    for (const auto& entry : data) {
        if (entry.second < minTemp) minTemp = entry.second;
        if (entry.second > maxTemp) maxTemp = entry.second;
    }
    float tempRange = maxTemp - minTemp;
    if (tempRange == 0) tempRange = 1;

    // Ajout de l’échelle de température (graduations horizontales)
    int nbGraduationsY = 5;
    for (int i = 0; i <= nbGraduationsY; ++i) {
        float value = minTemp + (tempRange * i / nbGraduationsY);
        float y = height - ((value - minTemp) / tempRange) * height;

        scene->addLine(0, y, width, y, QPen(Qt::lightGray));
        scene->addText(QString::number(value, 'f', 1) + " °C")->setPos(0, y - 10);
    }

    // Ajout de l’échelle de dates (graduations verticales à partir des données)
    int step = std::max<size_t>(1, data.size() / 5); // environ 5 graduations
    for (size_t i = 0; i < data.size(); i += step) {
        float x = static_cast<float>(width) * i / (data.size() - 1);
        QDateTime tickDate = data[i].first;

        scene->addLine(x, 0, x, height, QPen(Qt::lightGray));
        scene->addText(tickDate.toString("dd/MM hh:mm"))->setPos(x, height - 20);
    }

    // Tracer les données
    for (size_t i = 1; i < data.size(); ++i) {
        float x1 = static_cast<float>(width) * (i - 1) / (data.size() - 1);
        float y1 = height - ((data[i - 1].second - minTemp) / tempRange) * height;
        float x2 = static_cast<float>(width) * i / (data.size() - 1);
        float y2 = height - ((data[i].second - minTemp) / tempRange) * height;
        scene->addLine(x1, y1, x2, y2, pen);
    }

	// Affiche la temperature moyenne avec un trait rouge
	float sumTemp = 0.0f;
	for (const auto& entry : data) {
		sumTemp += entry.second;
	}
	float avgTemp = sumTemp / data.size();
	float yAvg = height - ((avgTemp - minTemp) / tempRange) * height;
	scene->addLine(0, yAvg, width, yAvg, QPen(Qt::red, 2, Qt::DashLine));


    // Adapter automatiquement le graphique à la vue
    ui.graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
	data.clear();
}