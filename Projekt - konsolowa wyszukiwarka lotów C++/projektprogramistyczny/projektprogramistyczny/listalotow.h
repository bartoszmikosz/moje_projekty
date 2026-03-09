#ifndef LISTALOTOW_H
#define LISTALOTOW_H

#include "klasalot.h"
#include "elementlisty.h"


class ListaLotow {
private:
	std::shared_ptr<ElListyInt> glowa;

public:
	static int rozmiar;
	ListaLotow();
	~ListaLotow();

	void dodaj(const Lot& lot);
	void usun(const Lot & lot);
	void wypisz() const;
	std::shared_ptr<ElListyInt> getGlowa() const;
	void kopiujzamiana(ListaLotow& b);
	ListaLotow& operator=(const ListaLotow& other);
};

#endif
