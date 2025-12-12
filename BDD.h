#pragma once
#include <vector>
#include <QDateTime>

//classe qui va se connecter a une BDD et faire des requetes SQL
class BDD
{
	public:
	BDD();
	~BDD();
	void connectToDB();
	void insertData(float temperature);
	std::vector<std::pair<QDateTime, float>> getDataBetweenDates(QDateTime startDate, QDateTime endDate);
};