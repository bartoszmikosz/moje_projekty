#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include "funkcje.h"
#include "klasalot.h"
#include "listalotow.h"
#include "kllotywgtypu.h"


void LoadTheFile(std::string nazwapliku, ListaLotow& ListaL, ListaLWgTypu<Regularny>& ListaR, ListaLWgTypu<Czarter>& ListaC)
{
    Regularny tymr;
    Czarter tymc;
    std::ifstream file(nazwapliku);
    std::string temp;
    ListaLotow ListaTemp;
    ListaLWgTypu<Regularny> ListaRTemp;
    ListaLWgTypu<Czarter> ListaCTemp;
    if (file)
    {
        while (file >> temp)
        {
            if (temp == "R")
            {
                file >> tymr.id;
                file.ignore();
                getline(file, tymr.miasto);
                getline(file, tymr.linialotow);
                file >> tymr.cena;
                file >> tymr.pozostale_miejsca;
                file >> tymr.dzien;
                file >> tymr.miesiac;
                file >> tymr.rok;
                file.ignore();
                getline(file, tymr.godzina);
                tymr.ozntypu = "R";
                ListaR.dodaj(tymr);
                ListaL.dodaj(tymr);
            }
            else if (temp == "C")
            {
                file >> tymc.id;
                file.ignore();
                getline(file, tymc.miasto);
                getline(file, tymc.linialotow);
                file >> tymc.cena;
                file >> tymc.pozostale_miejsca;
                file >> tymc.dzien;
                file >> tymc.miesiac;
                file >> tymc.rok;
                file.ignore();
                getline(file, tymc.godzina);
                tymc.ozntypu = "C";
                ListaC.dodaj(tymc);
                ListaL.dodaj(tymc);
            }

        }
    }
    file.close();
    ListaTemp = ListaL;
    ListaTemp.kopiujzamiana(ListaL);
    ListaRTemp = ListaR;
    ListaRTemp.kopiujzamiana(ListaR);
    ListaCTemp = ListaC;
    ListaCTemp.kopiujzamiana(ListaC);
}


void MenuFiltrujace(ListaLotow& loty, ListaLWgTypu<Regularny>& lotyr, ListaLWgTypu<Czarter>& lotyc)
{
    ListaLotow PFiltrowaniu;
    ListaLWgTypu<Regularny> PFiltrowaniuR;
    ListaLWgTypu<Czarter> PFiltrowaniuC;
    PFiltrowaniu = loty;
    PFiltrowaniuR = lotyr;
    PFiltrowaniuC = lotyc;
    int stan = 0;
    while (true)
    {
        std::string wyborstr;
        std::cout << "1. Pokaz loty" << std::endl;
        std::cout << "2. Wyszukuj na podstawie ceny" << std::endl;
        std::cout << "3. Wyszukuj na podstawie lokalizacji" << std::endl;
        std::cout << "4. Wyszukuj na podstawie typu" << std::endl;
        std::cout << "5. Powrot do menu glownego" << std::endl;
        std::cin >> wyborstr;
        if (wyborstr == "1")
        {
            if (stan == 0)
                PFiltrowaniu.wypisz();
            else if (stan == 1)
                PFiltrowaniuR.wypisz();
            else if (stan == 2)
                PFiltrowaniuC.wypisz();
        }
        else if (wyborstr == "2")
        {
            auto filtrowaniewgceny = [](ListaLotow& zrodlowa, ListaLWgTypu<Regularny>& zrodlowar, ListaLWgTypu<Czarter>& zrodlowac)
            {
					double min, max;
					std::string wyborstr;
					std::cout << "Podaj najnizsza cene: " << std::endl;
					std::cin >> wyborstr;
					int strblad = 0;
					for (int i = 0; i < wyborstr.size(); i++)
					{
						if ((wyborstr[i] < 48 && wyborstr[i] != 46) || wyborstr[i]>57)
						{
							strblad++;
							min = 100;
							break;
						}
					}
					if (strblad == 0)
						min = stoi(wyborstr);
					else
					{
						std::cout << "To nie jest liczba! Podaj dodatnia liczbe calkowita" << std::endl;
						return;
					}
					std::cout << "Podaj najwyzsza cene: " << std::endl;
					std::cin >> wyborstr;
					for (int i = 0; i < wyborstr.size(); i++)
					{
						if ((wyborstr[i] < 48 && wyborstr[i] != 46) || wyborstr[i] > 57)
						{
							strblad++;
							max = 100;
							break;
						}
					}
					if (strblad == 0)
						max = stoi(wyborstr);
					else
					{
						std::cout << "To nie jest liczba! Podaj dodatnia liczbe calkowita" << std::endl;
						return;
					}
					std::shared_ptr<ElListyInt> biezacy = zrodlowa.getGlowa();
					std::shared_ptr<ElListyInt> tempi = nullptr;

					while (biezacy)
					{
						Lot temp;
						temp = biezacy->lot;
						if (temp.cena < min || temp.cena > max)
						{
							if (temp.ozntypu == "R")
							{
								auto biezacyk = zrodlowar.getGlowa();
								std::shared_ptr<ElListyLInt<Regularny>> tempik = nullptr;
								while (biezacyk)
								{
									if (biezacyk->lot.id == temp.id)
									{
										zrodlowar.usun(biezacyk->lot);
										break;
									}
									tempik = biezacyk;
									biezacyk = biezacyk->nastepny;
								}
							}
							else if (temp.ozntypu == "C")
							{
								auto biezacyz = zrodlowac.getGlowa();
								std::shared_ptr<ElListyLInt<Czarter>> tempiz = nullptr;
								while (biezacyz)
								{
									if (biezacyz->lot.id == temp.id)
									{
										zrodlowac.usun(biezacyz->lot);
										break;
									}
									tempiz = biezacyz;
									biezacyz = biezacyz->nastepny;
								}
							}
							zrodlowa.usun(temp);
						}
						else
						{
							tempi = biezacy;
						}
						biezacy = tempi ? tempi->nastepny : zrodlowa.getGlowa();
					}
            };
            filtrowaniewgceny(PFiltrowaniu, PFiltrowaniuR, PFiltrowaniuC);
        }
		else if (wyborstr == "3")
		{
			auto filtrowaniewgmiasta = [](ListaLotow& zrodlowa, ListaLWgTypu<Regularny>& zrodlowar, ListaLWgTypu<Czarter>& zrodlowac)
				{
					std::string city = "1";
					std::vector<std::string> citysearch;
					while (city != "0")
					{
						std::cout << "Podaj miasta docelowe ktore maja sie znalezc w wynikach: (jesli chcesz zakonczyc napisz 0)" << std::endl;
						std::cin >> city;
						if (city != "0")
							citysearch.push_back(city);
					}

					std::shared_ptr<ElListyInt> biezacy = zrodlowa.getGlowa();
					std::shared_ptr<ElListyInt> tempi = nullptr;

					while (biezacy)
					{
						Lot temp;
						temp = biezacy->lot;
						int a = 0;
						for (const auto& miasto : citysearch)
						{
							if (temp.miasto == miasto)
								a++;
						}
						if (a == 0)
						{
							if (temp.ozntypu == "R")
							{
								auto biezacyk = zrodlowar.getGlowa();
								std::shared_ptr<ElListyLInt<Regularny>> tempik = nullptr;
								while (biezacyk)
								{
									if (biezacyk->lot.id == temp.id)
									{
										zrodlowar.usun(biezacyk->lot);
										break;
									}
									tempik = biezacyk;
									biezacyk = biezacyk->nastepny;
								}
							}
							else if (temp.ozntypu == "C")
							{
								auto biezacyz = zrodlowac.getGlowa();
								std::shared_ptr<ElListyLInt<Czarter>> tempiz = nullptr;
								while (biezacyz)
								{
									if (biezacyz->lot.id == temp.id)
									{
										zrodlowac.usun(biezacyz->lot);
										break;
									}
									tempiz = biezacyz;
									biezacyz = biezacyz->nastepny;
								}
							}
							zrodlowa.usun(temp);
						}
						else
						{
							tempi = biezacy;
						}
						biezacy = tempi ? tempi->nastepny : zrodlowa.getGlowa();
					}

				};
			filtrowaniewgmiasta(PFiltrowaniu, PFiltrowaniuR, PFiltrowaniuC);
		}
		else if (wyborstr == "4")
		{
			auto filtrowaniewgtypu = [](int& stan)
			{
					std::string strtemp;
					std::cout << "Jakiego typu chcesz wyswietlac loty" << std::endl;
					std::cout << "1 - regularne " << std::endl;
					std::cout << "2 - czartery " << std::endl;
					std::cin >> strtemp;
					if (strtemp == "1")
						stan = 1;
					else if (strtemp == "2")
						stan = 2;
					else
						std::cout << "Bledny wybor" << std::endl;
					return;
			};
			filtrowaniewgtypu(stan);
			
		}
		else if (wyborstr == "5")
		{
			std::cout << "Wracam do menu glownego: " << std::endl;
			break;
		}
		else
		{
			std::cout << "Niepoprawny wybor, wybierz ponownie." << std::endl;
		}
        
    }
}

void ZapiszPlikBazowy(std::string nazwapliku, ListaLotow& ListaL)
{
	std::ofstream plik(nazwapliku);
	if (plik)
	{
		std::shared_ptr<ElListyInt> biezacy = ListaL.getGlowa();
		while (biezacy)
		{
			plik << biezacy->lot.ozntypu << std::endl;
			plik << biezacy->lot.id << std::endl;
			plik << biezacy->lot.miasto << std::endl;
			plik << biezacy->lot.linialotow << std::endl;
			plik << std::fixed << std::setprecision(2) << biezacy->lot.cena << std::endl;
			plik << biezacy-> lot.pozostale_miejsca << std::endl;
			plik << biezacy->lot.dzien << std::endl;
			plik << biezacy->lot.miesiac << std::endl;
			plik << biezacy->lot.rok << std::endl;
			plik << biezacy->lot.godzina << std::endl;
			biezacy = biezacy->nastepny;
		}
	}
	plik.close();
}


void RezerwujLot(ListaLotow& ListaL, ListaLWgTypu<Regularny>& ListaR, ListaLWgTypu<Czarter>& ListaC, std::string nazwapliku)
{
	auto aktualizuj_id = [](auto& lista, int idusun)
	{
			auto biezacy = lista.getGlowa();
			while (biezacy)
			{
				if (biezacy->lot.id > idusun)
				{
					biezacy->lot.id--;
				}
				biezacy = biezacy->nastepny;
			}
	};
	int idlotu = 0;
	std::string stridlotu = "";
	int ilmiejsc = 0;
	std::string strilmiejsc = "";
	std::cout << "Podaj numer id lotu do ktorego chcesz zarezerwowac miejsce (miejsca)" << std::endl;
	std::cin >> stridlotu;
	int strblad = 0;
	for (int i = 0; i < stridlotu.size(); i++)
	{
		if (stridlotu[i] < 48 || stridlotu[i]>57)
		{
			strblad++;
			break;
		}
	}
	if (strblad == 0)
		idlotu = stoi(stridlotu);
	else
	{
		std::cout << "To nie jest liczba! Podaj dodatnia liczbe calkowita" << std::endl;
		return;
	}
	strblad = 0;
	std::cout << "Podaj ilosc miejsc ktore chcesz zarezerwowac" << std::endl;
	std::cin >> strilmiejsc;
	for (int i = 0; i < strilmiejsc.size(); i++)
	{
		if (strilmiejsc[i] < 48 || strilmiejsc[i]>57)
		{
			strblad++;
			break;
		}
	}
	if (strblad == 0)
		ilmiejsc = stoi(strilmiejsc);
	else
	{
		std::cout << "To nie jest liczba! Podaj dodatnia liczbe calkowita" << std::endl;
		return;
	}
	std::shared_ptr<ElListyInt> biezacy = ListaL.getGlowa();
	std::shared_ptr<ElListyInt> tempi = nullptr;

	while (biezacy)
	{
		Lot temp;
		temp = biezacy->lot;
		if (temp.id == idlotu)
		{
			if (temp.ozntypu == "R")
			{
				auto biezacyk = ListaR.getGlowa();
				std::shared_ptr<ElListyLInt<Regularny>> tempik = nullptr;
				while (biezacyk)
				{
					if (biezacyk->lot.id == temp.id)
					{
						if (ilmiejsc < biezacyk->lot.pozostale_miejsca)
						{
							biezacyk->lot.pozostale_miejsca -= ilmiejsc;
						}
						else if (ilmiejsc == biezacyk->lot.pozostale_miejsca)
						{
							biezacyk->lot.pozostale_miejsca -= ilmiejsc;
							ListaR.usun(temp);
							aktualizuj_id(ListaR, temp.id);
						}
						else
						{
							std::cout << "Podano zbyt duza liczbe miejsc." << std::endl;
							return;
						}
						break;
					}
					tempik = biezacyk;
					biezacyk = biezacyk->nastepny;
				}
			}
			else if (temp.ozntypu == "C")
			{
				auto biezacyz = ListaC.getGlowa();
				std::shared_ptr<ElListyLInt<Czarter>> tempiz = nullptr;
				while (biezacyz)
				{
					if (biezacyz->lot.id == temp.id)
					{
						if (ilmiejsc < biezacyz->lot.pozostale_miejsca)
						{
							biezacyz->lot.pozostale_miejsca -= ilmiejsc;
						}
						else if (ilmiejsc == biezacyz->lot.pozostale_miejsca)
						{
							biezacyz->lot.pozostale_miejsca -= ilmiejsc;
							ListaC.usun(temp);
							aktualizuj_id(ListaC, temp.id);
						}
						else
						{
							std::cout << "Podano zbyt duza liczbe miejsc." << std::endl;
							return;
						}
						break;
					}
					tempiz = biezacyz;
					biezacyz = biezacyz->nastepny;
				}
			}
			if (ilmiejsc < biezacy->lot.pozostale_miejsca)
			{
				biezacy->lot.pozostale_miejsca -= ilmiejsc;
				std::cout << "Miejsca zostaly zarezerwowane." << std::endl;
			}
			else if (ilmiejsc == biezacy->lot.pozostale_miejsca)
			{
				biezacy->lot.pozostale_miejsca -= ilmiejsc;
				ListaL.usun(temp);
				aktualizuj_id(ListaL, temp.id);
				std::cout << "Miejsca zostaly zarezerwowane. Byly to ostatnie miejsca wiec wpis zostal usuniety" << std::endl;
			}
			else
			{
				std::cout << "Podano zbyt duza liczbe miejsc." << std::endl;
				return;
			}
			ZapiszPlikBazowy(nazwapliku, ListaL);
			return;
		}
		else
		{
			tempi = biezacy;
		}
		biezacy = tempi ? tempi->nastepny : ListaL.getGlowa();
	}
	if (!biezacy)
	{
		std::cout << "Nie znaleziono prawidlowego id! Miejsca nie zostaly zarezerwowane" << std::endl;
		return;
	}
}

void Ciekawostki(ListaLotow& ListaL, ListaLWgTypu<Regularny>& ListaR, ListaLWgTypu<Czarter>& ListaC)
{
	std::vector<double> ceny;
	std::vector<std::string> loty1;
	std::vector<std::string> loty2;
	std::vector<std::string> loty;
	std::shared_ptr<ElListyInt> biezacy = ListaL.getGlowa();
	std::shared_ptr<ElListyInt> tempi = nullptr;
	while (biezacy)
	{
		ceny.push_back(biezacy->lot.cena);
		loty.push_back(biezacy->lot.linialotow);
		biezacy = biezacy->nastepny;
	}
	auto biezacyr = ListaR.getGlowa();
	std::shared_ptr<ElListyLInt<Regularny>> tempir = nullptr;
	while (biezacyr)
	{
		loty1.push_back(biezacyr->lot.miasto);
		biezacyr = biezacyr->nastepny;
	}
	auto biezacyc = ListaC.getGlowa();
	std::shared_ptr<ElListyLInt<Czarter>> tempic = nullptr;
	while (biezacyc)
	{
		loty2.push_back(biezacyc->lot.miasto);
		biezacyc = biezacyc->nastepny;
	}
	std::vector<double>::iterator iter1 = ceny.begin();
	std::vector<double>::iterator iter2 = ceny.end();
	std::vector<std::string>::iterator iter3 = loty.begin();
	std::vector<std::string>::iterator iter4 = loty.end();
	std::vector<std::string>::iterator iter5 = loty1.begin();
	std::vector<std::string>::iterator iter6 = loty1.end();
	std::vector<std::string>::iterator iter7 = loty2.begin();
	std::vector<std::string>::iterator iter8 = loty2.end();
	std::cout << "Oto kilka ciekawostek na temat podanej bazy danych!" << std::endl;
	std::vector<double>::iterator iter9 = ceny.begin();
	iter9 = std::max_element(iter1, iter2);
	std::cout << "Cena najdrozszego biletu w bazie wynosi " << *iter9 << " zl a jest on dla lotu o id rownym " << std::distance(iter1, iter9)+1 << std::endl;
	double a = std::accumulate(iter1, iter2, 0.0);
	std::cout << "Gdyby jakas osoba miala kupic po jednym bilecie z kazdego lotu, zaplacilaby " << a << " zl." << std::endl;
	bool czyjest = std::binary_search(iter3, iter4, "WizzAir");
	if (czyjest)
		std::cout << "W bazie znajduja sie loty linii WizzAir" << std::endl;
	else
		std::cout << "W bazie nie znajduja sie loty linii WizzAir" << std::endl;
	auto iter10 = std::unique(iter5, iter6);
	loty1.erase(iter10, loty1.end());
	std::sort(iter5, iter6);
	auto iter11 = std::unique(iter7, iter8);
	loty2.erase(iter11, loty2.end());
	std::sort(iter7, iter8);
	std::cout << "Wszystkie miasta docelowe lotow regularnych to: " << std::endl;
	for (int i = 0; i < loty1.size(); i++)
	{
		std::cout << loty1[i] << " ";
	}
	std::cout << std::endl << "a wszystkie miasta docelowe lotow czarterowych to: " << std::endl;
	for (int i = 0; i < loty2.size(); i++)
	{
		std::cout << loty2[i] << " ";
	}
	std::cout << std::endl;




}

void DodajLot(ListaLotow& ListaL, ListaLWgTypu<Regularny>& ListaR, ListaLWgTypu<Czarter>& ListaC, std::string nazwapliku)
{
	Lot tmp;
	std::string tempstring = "";
	std::cin.ignore();
	std::cout << "Podaj nazwe miasta docelowego: " << std::endl;
	std::getline(std::cin, tempstring);
	tmp.miasto = tempstring;
	std::cout << "Podaj nazwe linii lotow: " << std::endl;
	std::getline(std::cin, tempstring);
	tmp.linialotow = tempstring;
	std::cout << "Podaj cene lotu: " << std::endl;
	std::cin >> tempstring;
	int strblad = 0;
	auto sprawdzczyblad = [] (int&strblad, std::string& tempstring, double& a) 
	{
			for (int i = 0; i < tempstring.size(); i++)
			{
				if ((tempstring[i] < 48 && tempstring[i] != 46) || tempstring[i] > 57)
				{
					strblad++;
					break;
				}
			}
			if (strblad == 0)
				a = stod(tempstring);
			else
			{
				std::cout << "Bledne dane wejsciowe!";
				strblad = 1;
				return;
			}
	};
	auto sprawdzczybladint = [](int& strblad, std::string& tempstring, int& a)
		{
			for (int i = 0; i < tempstring.size(); i++)
			{
				if (tempstring[i] < 48 || tempstring[i] > 57)
				{
					strblad++;
					break;
				}
			}
			if (strblad == 0)
				a = stoi(tempstring);
			else
			{
				std::cout << "Bledne dane wejsciowe!";
				strblad = 1;
				return;
			}
		};
	sprawdzczyblad(strblad, tempstring, tmp.cena);
	if (strblad == 1)
		return;
	std::cout << "Podaj dzien lotu" << std::endl;
	std::cin >> tempstring;
	sprawdzczybladint(strblad, tempstring, tmp.dzien);
	if (strblad == 1)
		return;
	std::cout << "Podaj miesiac lotu" << std::endl;
	std::cin >> tempstring;
	sprawdzczybladint(strblad, tempstring, tmp.miesiac);
	if (strblad == 1)
		return;
	std::cout << "Podaj rok lotu" << std::endl;
	std::cin >> tempstring;
	sprawdzczybladint(strblad, tempstring, tmp.rok);
	if (strblad == 1)
		return;
	std::cout << "Podaj liczbe wolnych miejsc w samolocie" << std::endl;
	std::cin >> tempstring;
	sprawdzczybladint(strblad, tempstring, tmp.pozostale_miejsca);
	if (strblad == 1)
		return;
	std::cin.ignore();
	std::cout << "Podaj godzine odlotu: " << std::endl;
	std::getline(std::cin, tempstring);
	tmp.godzina = tempstring;
	std::cout << "Podaj typ lotu - R dla regularnego, C dla czarteru: " << std::endl;
	std::getline(std::cin, tempstring);
	if (tempstring != "R" && tempstring != "C")
	{
		std::cout << "Bledny typ!" << std::endl;
		return;
	}
	tmp.ozntypu = tempstring;
	tmp.id = ListaL.rozmiar + 1;
	ListaLotow ListaTemp;
	ListaL.kopiujzamiana(ListaTemp);
	ListaTemp.dodaj(tmp);
	ListaTemp.kopiujzamiana(ListaL);

	auto dodaj_do_listy = [&tmp](auto& lista, auto mieszkanie, auto& lista2)
	{
			lista.kopiujzamiana(lista2);
			lista2.dodaj(mieszkanie);
			lista2.kopiujzamiana(lista);
	};
	ListaLWgTypu<Regularny> tmpr;
	ListaLWgTypu<Czarter> tmpc;
	if (tmp.ozntypu == "R")
	{
		dodaj_do_listy(ListaR, Regularny(tmp), tmpr);
	}
	else if (tmp.ozntypu == "C")
	{
		dodaj_do_listy(ListaC, Czarter(tmp), tmpc);
	}
	ZapiszPlikBazowy(nazwapliku, ListaL);
	std::cout << "Dodano lot." << std::endl;
}

void UsunLot(ListaLotow& ListaL, ListaLWgTypu<Regularny>& ListaR, ListaLWgTypu<Czarter>& ListaC, std::string nazwapliku)
{
	int idusun;
	std::string idtemp;
	std::cout << "Podaj id lotu ktore ma zostac usuniete:" << std::endl;
	std::cin >> idtemp;
	int strblad = 0;
	for (int i = 0; i < idtemp.size(); i++)
	{
		if (idtemp[i] < 48 || idtemp[i] > 57)
		{
			strblad++;
			break;
		}
	}
	if (strblad == 0)
		idusun = stoi(idtemp);
	else
	{
		std::cout << "Bledne wprowadzone dane!" << std::endl;
		return;
	}
	std::shared_ptr<ElListyInt> biezacy = ListaL.getGlowa();
	while (biezacy && biezacy->lot.id != idusun)
	{
		biezacy = biezacy->nastepny;
	}
	if (!biezacy)
	{
		std::cout << "Nie znaleziono takiego id! " << std::endl;
		return;
	}
	if (biezacy->lot.id == idusun)
	{
		auto usun_z_listy = [idusun](auto& lista)
			{
				auto biezacy = lista.getGlowa();
				while (biezacy)
				{
					if (biezacy->lot.id == idusun)
					{
						lista.usun(biezacy->lot);
						break;
					}
					biezacy = biezacy->nastepny;
				}
			};

		if (biezacy->lot.ozntypu == "R")
		{
			usun_z_listy(ListaR);
		}
		else if (biezacy->lot.ozntypu == "Z")
		{
			usun_z_listy(ListaC);
		}
		ListaL.usun(biezacy->lot);
		auto aktualizuj_id = [idusun](auto& lista)
			{
				auto biezacy = lista.getGlowa();
				while (biezacy)
				{
					if (biezacy->lot.id > idusun)
					{
						biezacy->lot.id--;
					}
					biezacy = biezacy->nastepny;
				}
			};

		aktualizuj_id(ListaL);
		aktualizuj_id(ListaR);
		aktualizuj_id(ListaC);
		ZapiszPlikBazowy(nazwapliku, ListaL);
		std::cout << "Usunieto lot." << std::endl;
		return;
	}
}