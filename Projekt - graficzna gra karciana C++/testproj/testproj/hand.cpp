#include "hand.h"

void Hand::AddCard(Card& card)
{
	reka.push_back(card);
}

void Hand::RemoveCard(int index)
{
	reka.erase(reka.begin()+index);
}