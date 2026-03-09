#include <iostream>
#include <string>
#include <fstream>
#include "funkcje.h"
#include "elementlisty.h"
#include "klasalot.h"
#include "listalotow.h"



int main()
{
    std::string nazwapliku = "baza.txt";
    ListaLotow loty;
    ListaLWgTypu<Regularny> lotyr;
    ListaLWgTypu<Czarter> lotyc;
    LoadTheFile(nazwapliku,loty,lotyr,lotyc);
    std::string wyborstr = "";
    while (wyborstr != "0")
    {
        std::cout << "Witaj uzytkowniku!" << std::endl;
        std::cout << "1 - wyszukuj loty" << std::endl;
        std::cout << "2 - zarezerwuj lot" << std::endl;
        std::cout << "3 - dodaj lub usun lot" << std::endl;
        std::cout << "4 - ciekawostki na temat bazy danych" << std::endl;
        std::cout << "0 - wyjdz z programu" << std::endl;
        std::cout << "Twoj wybor: " << std::endl;
        std::cin >> wyborstr;
        if (wyborstr == "1")
        {
            MenuFiltrujace(loty, lotyr, lotyc);
        }
        else if (wyborstr == "2")
        {
            RezerwujLot(loty, lotyr, lotyc, nazwapliku);
        }
        else if (wyborstr == "3")
        {
            std::string wybo;
            std::cout << "Podaj czy chcesz dodac czy usunac lot -> 1 jezeli dodac, 2 jezeli usunac:" << std::endl;
            std::cin >> wybo;
            if (wybo == "1")
                DodajLot(loty, lotyr, lotyc, nazwapliku);
            else if (wybo == "2")
            {
                UsunLot(loty, lotyr, lotyc, nazwapliku);
            }
            else
                std::cout << "Bledny wybor" << std::endl;



        }
        else if (wyborstr == "4")
        {
            Ciekawostki(loty, lotyr, lotyc);
        }
        else if (wyborstr == "0")
        {
            std::cout << "Dziekujemy za skorzytanie z wyszukiwarki lotow";
            break;
        }
        else
        {
            std::cout << "Wybor nieprawidlowy, prosimy wybrac ponownie";
        }
    }
    return 0;
}

