#pragma once

#include <iostream>
#include <vector>
#include <raylib.h>
#include "button.h"

class Deck {

private:
	
public:
	std::vector<Card> talia;
	void AddCards(Card& karta);
	void Remove_Card(int index);
	int Get_Size();
	Card Get_Card(int index);
	void Clear();
};
