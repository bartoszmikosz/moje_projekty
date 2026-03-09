#ifndef LISTATYPYMIESZKAN_H
#define LISTATYPYMIESZKAN_H

#include <iostream>
#include <memory>
#include "klasalot.h"
#include "elementlisty.h"

template<typename T>
class ListaLWgTypu {
private:
    std::shared_ptr<ElListyLInt<T>> glowa;

public:
    static int rozmiar;
    ListaLWgTypu();
    ~ListaLWgTypu();
    std::shared_ptr<ElListyLInt<T>> getGlowa() const;
    void dodaj(const T& lot);
    void usun(const T& lot);
    void wypisz() const;
    void kopiujzamiana(ListaLWgTypu<T>& b);
    ListaLWgTypu<T>& operator=(const ListaLWgTypu<T>& other);
};

template<typename T>
int ListaLWgTypu<T>::rozmiar = 0;

template<typename T>
ListaLWgTypu<T>::ListaLWgTypu() : glowa(nullptr) {

    rozmiar = 0;
}

template<typename T>
ListaLWgTypu<T>::~ListaLWgTypu() {
    while (glowa != nullptr) {
        glowa = glowa->nastepny;
    }
}

template<typename T>
void ListaLWgTypu<T>::dodaj(const T& kllot) {
    glowa = std::make_shared<ElListyLInt<T>>(kllot, glowa);
    rozmiar++;
}

template<typename T>
void ListaLWgTypu<T>::usun(const T& kllot) {
    std::shared_ptr<ElListyLInt<T>> poprzedni = nullptr;
    std::shared_ptr<ElListyLInt<T>> obecny = glowa;

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

template <typename T>
void ListaLWgTypu<T>::wypisz() const {
    std::shared_ptr<ElListyLInt<T>> obecny = glowa;

    while (obecny != nullptr) {
        std::cout << obecny->lot;
        if (dynamic_cast<Regularny*>(&obecny->lot))
            std::cout << std::endl << "LOT REGULARNY";
        else if (dynamic_cast<Czarter*>(&obecny->lot))
            std::cout << std::endl << "LOT CZARTEROWY";
        std::cout << std::endl << std::endl;
        obecny = obecny->nastepny;
    }
}

template <typename T>
std::shared_ptr<ElListyLInt<T>> ListaLWgTypu<T>::getGlowa() const {
    return glowa;
}


template <typename T>
void ListaLWgTypu<T>::kopiujzamiana(ListaLWgTypu<T>& b)
{
    int a = b.rozmiar;
    std::shared_ptr<ElListyLInt<T>> biezacy = glowa;
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

template <typename T>
ListaLWgTypu<T>& ListaLWgTypu<T>::operator=(const ListaLWgTypu<T>& other) {
    if (this != &other) {
        glowa = nullptr;
        rozmiar = 0;
        std::shared_ptr<ElListyLInt<T>> otherCurrent = other.glowa;
        std::shared_ptr<ElListyLInt<T>> thisPrevious = nullptr;
        while (otherCurrent) {
            std::shared_ptr<ElListyLInt<T>> newElement = std::make_shared<ElListyLInt<T>>(otherCurrent->lot);
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


#endif
