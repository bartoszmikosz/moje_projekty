#ifndef KLASAMIESZKANIA_H
#define KLASAMIESZKANIA_H

#include <iostream>


class Lot {
public:
	int id = 0;
	std::string miasto = "";
	std::string linialotow = "";
	double cena = 0;
	int pozostale_miejsca = 10;
	int dzien = 0;
	int miesiac = 0;
	int rok = 0;
	std::string godzina = "";
	std::string ozntypu = "";
	Lot();
	friend bool operator==(const Lot& l1, const Lot& l2);
	friend std::ostream& operator<<(std::ostream& os, const Lot& z);
	virtual ~Lot() {};
};

class Regularny : public Lot
{
public:
	Regularny();
	Regularny(Lot& m);
};

class Czarter : public Lot
{
public:
	Czarter();
	Czarter(Lot& m);
};

#endif
