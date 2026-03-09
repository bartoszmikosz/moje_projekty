#include "funkcje.h"

namespace fs = std::filesystem;


void ReloadDeck(Deck& gameDeck)
{
	gameDeck.Clear();
    std::string pom = "";
    std::string data = "";
    for (int i = 10; i > 1; i--)
    {

        pom = std::to_string(i);
        data = "Graphics/card";
        data += pom;
        data += "spades.png";
        std::cout << data;
        Card baba{ data.c_str(), {300,300}, 0.6, 10 };
        baba.SetPath(data);
        baba.SetPoints(i);
        gameDeck.AddCards(baba);
    }

    for (int i = 10; i > 1; i--)
    {


        pom = std::to_string(i);
        data = "Graphics/card";
        data += pom;
        data += "hearts.png";
        std::cout << data;
        Card baba{ data.c_str(), {300,300}, 0.6, 10 };
        baba.SetPath(data);

        baba.SetPoints(i);

        gameDeck.AddCards(baba);


    }
    for (int i = 10; i > 1; i--)
    {


        pom = std::to_string(i);
        data = "Graphics/card";
        data += pom;
        data += "leaves.png";
        std::cout << data;
        Card baba{ data.c_str(), {300,300}, 0.6, 10 };
        baba.SetPath(data);

        baba.SetPoints(i);

        gameDeck.AddCards(baba);


    }
    for (int i = 10; i > 1; i--)
    {


        pom = std::to_string(i);
        data = "Graphics/card";
        data += pom;
        data += "clovers.png";
        std::cout << data;
        Card baba{ data.c_str(), {300,300}, 0.6, 10 };
        baba.SetPath(data);

        baba.SetPoints(i);

        gameDeck.AddCards(baba);


    }
}

std::string AssertPremPoints(int& points, std::vector<Card>& table)
{
    int how_many = 0;
    int how_many_max = 0;
    int number_of_pairs = 0;
   
    PLAY_TYPE type = REGULAR;
    for (int i = 0; i < table.size(); i++)
    {
        for (int j = 0; j < table.size(); j++)
        {
            if (table[i].GetPath()[13] == table[j].GetPath()[13])
            {
                how_many++;
                if (how_many > how_many_max)
                {
                    how_many_max = how_many;
                }
            }
        }
        if (how_many == 2)
            number_of_pairs++;
        how_many = 0;
       
    }
    if (how_many_max == 4)
         type = QUADRUPLE;
    else if (how_many_max >= 2 && number_of_pairs == 4)
        type = TWO_PAIRS;
    else if (how_many_max == 3)
        type = TRIPLE;
    else if (how_many_max == 2)
        type = PAIR;
   
    table.clear();

    if (type == PAIR)
    {
        points = points * 1.5;
        return "PAIR";
    }  
    else if (type == TRIPLE)
    {
        points *= 2.5;
        return "TRIPLE";
    }   
    else if (type == TWO_PAIRS)
    {
        points *= 3;
        return "TWO PAIRS";
    }   
    else if (type == QUADRUPLE)
    {
        points *= 5;
        return "QUADRUPLE";
    }
    else
    {
        return "REGULAR";
    }
       
    

        
}

void Serve_Extra_Points(bool tab[5], std::string type, int& points, int discards_number)
{
    if (tab[1])
    {
        points = 1.25 * points;
        return;
    }
    else if (tab[2])
    {
        if (type == "TRIPLE")
            points *= 3;
        return;
    }
    else if (tab[3])
    {
        if (type == "PAIR")
            points *= 2;
        return;
    }
    else if (tab[4])
    {
        points = points + 5 * discards_number;
    }

}

/* 
void HandleWinners(const std::string& name, int score) {
    std::regex name_regex("^[A-Za-z]{1,19}$"); // tylko litery
    if (!std::regex_match(name, name_regex)) {
        std::cout << "Nieprawidlowe imie. Tylko litery, bez spacji. Pomijam zapis.\n";
        return;
    }

    std::string filename = "leaderboard.txt";
    if (!fs::exists(filename)) {
        std::ofstream createFile(filename);
        createFile.close();
    }

    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << name << " " << score << "\n";
        file.close();
    }
}

void ShowLeaderboard() {
    std::ifstream file("leaderboard.txt");
    if (!file.is_open()) {
        DrawText("Brak pliku leaderboard.txt", 100, 100, 30, RED);
        return;
    }

    std::string name;
    int score;
    std::vector<std::pair<std::string, int>> entries;

    while (file >> name >> score) {
        entries.emplace_back(name, score);
    }

    std::sort(entries.begin(), entries.end(), [](auto& a, auto& b) {
        return a.second > b.second;
        });

    ClearBackground(BLACK);
    DrawText("TABLICA WYNIKOW:", 100, 40, 40, LIGHTGRAY);

    for (size_t i = 0; i < entries.size() && i < 10; ++i) {
        DrawText(TextFormat("%zu. %s - %i", i + 1, entries[i].first.c_str(), entries[i].second),
            100, 100 + i * 40, 30, LIGHTGRAY);
    }

    DrawText("ENTER - Wroc do menu", 100, 550, 25, LIGHTGRAY);
}

*/