#pragma once

#include "button.h"
#include <iostream>
#include <vector>


class Hand
{
	int amount_in_hand;
public:
	std::vector<Card> reka;
	std::vector<int> xpositions;
	std::vector<int> ypositions;
	void AddCard(Card& card);
	void RemoveCard(int index);

};
