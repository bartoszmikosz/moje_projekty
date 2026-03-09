export module leaderboard;

export import <string>;
export import <vector>;
export import <fstream>;
export import <iostream>;
export import <algorithm>;
export import <regex>;
export import <filesystem>;
export import <raylib.h>;

namespace fs = std::filesystem;



// Concept: tylko typy podobne do std::string
export template<typename T>
concept StringLike = requires(T a) {
    { a.c_str() } -> std::same_as<const char*>;
};

// Funkcja zapisuj¹ca wynik gracza
export template<StringLike T>
void HandleWinners(T name, int score) {
    std::regex name_regex("^[A-Za-z]{1,19}$");
    if (!std::regex_match(name, name_regex)) {
        std::cout << "Nieprawidlowe imie. Pomijam zapis.\n";
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
    }
}

// Funkcja wyœwietlaj¹ca leaderboard
export void ShowLeaderboard() {
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

    ClearBackground(DARKBLUE);
    DrawText("TABLICA WYNIKOW:", 100, 40, 40, LIGHTGRAY);

    for (size_t i = 0; i < entries.size() && i < 10; ++i) {
        DrawText(TextFormat("%zu. %s - %i", i + 1, entries[i].first.c_str(), entries[i].second),
            100, 100 + i * 40, 30, LIGHTGRAY);
    }

    DrawText("ENTER - Wroc do menu", 100, 550, 25, LIGHTGRAY);
}
