#include "deck.h"

void Deck::AddCards(Card& karta)
{
	talia.push_back(karta);
}

int Deck::Get_Size()
{
	return talia.size();
}

Card Deck::Get_Card(int index)
{
	return talia[index];
}

void Deck::Remove_Card(int index)
{
	talia.erase(talia.begin() + index);
}

void Deck::Clear()
{
	talia.clear();
}

