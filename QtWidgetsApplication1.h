#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication1.h"
#include <BDD.h>

class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget *parent = nullptr);
    ~QtWidgetsApplication1();
	friend bool tempLoop(QtWidgetsApplication1* window);

private:
    Ui::QtWidgetsApplication1Class ui;

public slots:
	void getGraphTemperature();
};

