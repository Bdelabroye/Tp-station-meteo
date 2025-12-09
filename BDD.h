#pragma once

//classe qui va se connecter a une BDD et faire des requetes SQL
class BDD
{
	public:
	BDD();
	~BDD();
	void connectToDB();
	void insertData(float temperature);
};