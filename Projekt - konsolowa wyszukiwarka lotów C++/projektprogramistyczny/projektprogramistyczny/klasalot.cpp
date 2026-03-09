#include <iostream>
#include <iomanip>
#include "klasalot.h"
#include "elementlisty.h"

Lot::Lot() : id(0), miasto(""), linialotow(""), cena(0), pozostale_miejsca(10), dzien(0), miesiac(0), rok(0), godzina("")
{

}

bool operator==(const Lot& l1, const Lot& l2)
{
	if (l1.id == l2.id)
		return true;
	else
		return false;
}

std::ostream& operator<<(std::ostream& os, const Lot& m) {
	std::cout << m.id << " " << m.miasto << " " << m.linialotow << " " << std::endl << std::fixed << std::setprecision(2) << m.cena << "zl" << " " << m.pozostale_miejsca << " miejsc zostalo"<< std::endl;
	std::cout << m.dzien << ".";
	if (m.miesiac < 10)
		std::cout << "0" << m.miesiac;
	else
		std::cout << m.miesiac;
	std::cout << "." << m.rok;
	std::cout << " "<< m.godzina << std::endl;
	return os;
}

Regularny::Regularny() : Lot()
{

}

//zastosowany jest domyslny konstruktor kopiujacy
Regularny::Regularny(Lot& m) : Lot(m)
{

}

Czarter::Czarter() : Lot()
{

}
Czarter::Czarter(Lot& m) : Lot(m)
{

}