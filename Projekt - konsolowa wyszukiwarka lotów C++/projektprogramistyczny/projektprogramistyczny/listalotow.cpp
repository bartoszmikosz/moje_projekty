#include <iostream>
#include <fstream>
#include <string>
#include "listalotow.h"


int ListaLotow::rozmiar = 0;

ListaLotow::ListaLotow() : glowa(nullptr) {

    rozmiar = 0;
}

ListaLotow::~ListaLotow() {
    while (glowa != nullptr) {
        glowa = glowa->nastepny;
    }
}

void ListaLotow::dodaj(const Lot& lot) {
    glowa = std::make_shared<ElListyInt>(lot, glowa);
    rozmiar++;
}

void ListaLotow::usun(const Lot& kllot) {
    std::shared_ptr<ElListyInt> poprzedni = nullptr;
    std::shared_ptr<ElListyInt> obecny = glowa;

    while (obecny != nullptr) {
        if (obecny->lot == kllot) {
            if (poprzedni == nullptr) {
                glowa = obecny->nastepny;
            }
            else {
                poprzedni->nastepny = obecny->nastepny;
            }
            break;
        }
        poprzedni = obecny;
        obecny = obecny->nastepny;
    }
    rozmiar--;
}

void ListaLotow::wypisz() const {
    std::shared_ptr<ElListyInt> obecny = glowa;

    while (obecny != nullptr) {
        std::cout << obecny->lot;
        if (obecny->lot.ozntypu == "R")
            std::cout << std::endl << "Lot regularny";
        else if (obecny->lot.ozntypu == "C")
            std::cout << std::endl << "Lot czarterowy";
        std::cout << std::endl << std::endl;
        obecny = obecny->nastepny;
    }
}

std::shared_ptr<ElListyInt> ListaLotow::getGlowa() const {
    return glowa;
}

void ListaLotow::kopiujzamiana(ListaLotow& b)
{
    int a = b.rozmiar;
    std::shared_ptr<ElListyInt> biezacy = glowa;
    if (this != &b) {
        b.glowa = nullptr;
        while (biezacy)
        {
            b.dodaj(biezacy->lot);
            biezacy = biezacy->nastepny;
        }
        b.rozmiar = a;
    }
    else
    {
        std::cout << "Probujesz skopiowac ten sam element! Operacja nie bedzie wykonana" << std::endl;
    }

}


ListaLotow& ListaLotow::operator=(const ListaLotow& other) {
    if (this != &other) {
        glowa = nullptr;
        rozmiar = 0;
        std::shared_ptr<ElListyInt> otherCurrent = other.glowa;
        std::shared_ptr<ElListyInt> thisPrevious = nullptr;
        while (otherCurrent) {
            std::shared_ptr<ElListyInt> newElement = std::make_shared<ElListyInt>(otherCurrent->lot);
            if (!thisPrevious)
                glowa = newElement;
            else
                thisPrevious->nastepny = newElement;
            thisPrevious = newElement;
            otherCurrent = otherCurrent->nastepny;
            rozmiar++;
        }
    }
    return *this;
}