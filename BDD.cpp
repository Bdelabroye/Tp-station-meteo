#include "BDD.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QVariant>
#include <qdebug>

BDD::BDD()
{
	this->connectToDB();
}

BDD::~BDD()
{
	if (QSqlDatabase::database().isOpen())
	{
		QSqlDatabase::database().close();
	}
}

void BDD::connectToDB()
{
	if (!QSqlDatabase::database().isOpen())
	{
		//connexion a la BDD MariaDB
		QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
		db.setHostName("172.29.19.33");
		db.setDatabaseName("temperature_db");
		db.setUserName("temp");
		db.setPassword("Dt7M7CWn/32@LOxg");
		if (!db.open()) {
			//erreur de connexion
		}
	}
}

void BDD::insertData(float temperature)
{
	//insertion de la temperature dans la BDD
	if (!QSqlDatabase::database().isOpen()) {
		qDebug() << "Database not open";
		return;
	}

	QSqlQuery q;
	q.prepare("INSERT INTO temperature_table (temperature, timestamp) "
		"VALUES (:temperature, :timestamp)");

	q.bindValue(":temperature", QVariant(temperature));
	q.bindValue(":timestamp", QDateTime::currentDateTime());

	if (!q.exec()) {
		qDebug() << "Insert failed:";
	}
	else {
		qDebug() << "Insert OK";
	}
}

std::vector<std::pair<QDateTime, float>> BDD::getDataBetweenDates(QDateTime startDate, QDateTime endDate)
{
	// recuperer les donnees entre les deux dates
	std::vector<std::pair<QDateTime, float>> data;
	if (!QSqlDatabase::database().isOpen()) {
		qDebug() << "Database not open";
		return data;
	}
	QSqlQuery q;
	q.prepare("SELECT timestamp, temperature FROM temperature_table "
		"WHERE timestamp BETWEEN :startDate AND :endDate "
		"ORDER BY timestamp ASC");
	q.bindValue(":startDate", startDate);
	q.bindValue(":endDate", endDate);
	if (!q.exec()) {
		qDebug() << "Select failed:";
		return data;
	}
	while (q.next()) {
		QDateTime timestamp = q.value(0).toDateTime();
		float temperature = q.value(1).toFloat();
		data.push_back(std::make_pair(timestamp, temperature));
	}
	//afficher les donnees recuperees 
	qDebug() << "Data retrieved:";
	for (const auto& entry : data) {
		qDebug() << "Timestamp:" << entry.first.toString() << "Temperature:" << entry.second;
	}
	return data;
}
