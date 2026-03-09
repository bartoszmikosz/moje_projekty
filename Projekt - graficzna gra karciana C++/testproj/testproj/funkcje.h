#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <regex>
#include <filesystem>
#include <raylib.h>
#include "button.h"
#include "deck.h"

typedef enum PLAY_TYPE { REGULAR, PAIR, TRIPLE, QUADRUPLE, STRAIGHT, TWO_PAIRS };

void ReloadDeck(Deck& GameDeck);


std::string AssertPremPoints(int& points, std::vector<Card>& table);

void Serve_Extra_Points(bool tab[5], std::string type, int& points, int discards_number);

