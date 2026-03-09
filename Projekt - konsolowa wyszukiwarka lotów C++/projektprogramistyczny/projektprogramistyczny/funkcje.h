#ifndef FUNKCJE_H
#define FUNKCJE_H

#include <iostream>
#include "listalotow.h"
#include "kllotywgtypu.h"

void LoadTheFile(std::string nazwapliku, ListaLotow& ListaL, ListaLWgTypu<Regularny>& ListaR, ListaLWgTypu<Czarter>& ListaC);
void MenuFiltrujace(ListaLotow& loty, ListaLWgTypu<Regularny>& lotyr, ListaLWgTypu<Czarter>& lotyc);
void ZapiszPlikBazowy(std::string nazwapliku, ListaLotow& ListaL);
void RezerwujLot(ListaLotow& ListaL, ListaLWgTypu<Regularny>& ListaR, ListaLWgTypu<Czarter>& ListaC, std::string nazwapliku);
void Ciekawostki(ListaLotow& ListaL, ListaLWgTypu<Regularny>& ListaR, ListaLWgTypu<Czarter>& ListaC);
void DodajLot(ListaLotow& ListaL, ListaLWgTypu<Regularny>& ListaR, ListaLWgTypu<Czarter>& ListaC, std::string nazwapliku);
void UsunLot(ListaLotow& ListaL, ListaLWgTypu<Regularny>& ListaR, ListaLWgTypu<Czarter>& ListaC, std::string nazwapliku);
#endif

