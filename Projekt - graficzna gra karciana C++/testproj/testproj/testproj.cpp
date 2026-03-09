#include <iostream>
#include <raylib.h>
#include <vector>
#include <string>
#include "button.h"
#include "deck.h"
#include "hand.h"
#include "funkcje.h"

import leaderboard;

using namespace std;

int main() {

    Color Dark_Green = Color{ 20, 160, 133, 255 };

    typedef enum GameScreen { MENU, GAMEPLAY, LOST, WON, POWER_UP, LEADERBOARD, TUTORIAL };
    typedef enum Movement { MOVE, NO_MOVE };

    const int screenWidth = 1500;
    const int screenHeight = 600;
    float card1x = 800;
    float card2x = 1100;
    int points = 0;
    int points2 = 0;
    int points_overall = 0;
    int points_overall_final = 0;
    int round_number = 1;
    int round_goal = 150;
    int hands_left = 5;
    int discards_left = 5;
    std::string stan;
    const char* power1substance ="";
    const char* power2substance ="";
    bool losulosu = false;
    int random = 0;
    int random2 = 0;
    char nameInput[20] = "";
    int letterCount = 0;


    InitWindow(screenWidth, screenHeight, "gra karciana");
    SetTargetFPS(60);

    Texture2D background = LoadTexture("Graphics/background.png");
    Button startButton{ "Graphics/button1.png", {100, 100}, 0.17 };
    Button exitButton{ "Graphics/button2.png", {100, 350}, 0.17 };
    Button leaderboardButton{ "Graphics/button3.png", {1100,100}, 0.17 };
    Button tutorialButton{ "Graphics/button4.png", {1100,350}, 0.17 };
    Button exitButton2{ "Graphics/button2.png", {100, 200}, 0.15 };
    Button confirmButton{ "Graphics/confirmbutton.png", {900, 220}, 0.7 };
    Button discardButton{ "Graphics/discardbutton.png", {1150, 220}, 0.7 };
    Rectangle boxA = { 50, 50, 700, 250 };
    Button powerA = { "Graphics/power1.png", {300, 100}, 1.2 };
    Button powerB = { "Graphics/power2.png", {300, 350}, 1.2 };
    Texture2D toriel = LoadTexture("Graphics/tutorial3.png");

    //string pom = "";
    Deck gameDeck;
    Hand gameHand;
    vector<Card> on_table;
    int hand_size = 6;
    ReloadDeck(gameDeck);

    
    bool exit = false;
    GameScreen currentscreen = MENU;
    Movement move = MOVE;
    bool reload_hand = true;
    std::vector<bool> pressed;
    std::vector<bool> collision_counted;
    std::vector<bool> to_delete;
    std::vector<Card> saved_cards;
    bool confirm = false;
    bool confirm2 = false;
    bool discard = false;
    bool reset = false;
    bool power[5] = { false };
    float time=0;
    int cards_in_hand = 0;

    while (WindowShouldClose() == false && exit == false)
    {
        Vector2 mousePosition = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        if (IsKeyDown(KEY_Q))
        {
            time++;
            if (time >= 180)
            {
               exit = true;
            }
        }
        if (IsKeyUp(KEY_Q))
        {
            time = 0;
        }
        if (startButton.isPressed(mousePosition, mousePressed) && currentscreen == MENU)
        {
            currentscreen = GAMEPLAY;
        }
        if (exitButton.isPressed(mousePosition, mousePressed) && currentscreen == MENU)
        {
            exit = true;
        }
        if (leaderboardButton.isPressed(mousePosition, mousePressed) && currentscreen == MENU)
        {
            currentscreen = LEADERBOARD;
        }
        if (tutorialButton.isPressed(mousePosition, mousePressed) && currentscreen == MENU)
        {
            currentscreen = TUTORIAL;
        }
        if (currentscreen == GAMEPLAY && reload_hand)
        {
           
            cards_in_hand = gameHand.reka.size();
            for (int i = 0; i < hand_size- cards_in_hand; i++)
            {
                if (gameDeck.Get_Size() == 0)
                {
                    ReloadDeck(gameDeck);
                }
                bool pressed_t = false;
                int randValue = GetRandomValue(0, gameDeck.Get_Size()-1);
                Card c{ gameDeck.Get_Card(randValue).GetPath().c_str(), 0.6, 10 };
                c.SetPath(gameDeck.Get_Card(randValue).GetPath());
                c.SetPoints(gameDeck.Get_Card(randValue).GetPoints());
                //c.GetNewImage(0.4);
                
                gameHand.AddCard(c);
                pressed.push_back(pressed_t);
                //collision.push_back(pressed_t);
                collision_counted.push_back(pressed_t);
                to_delete.push_back(pressed_t);
                gameDeck.Remove_Card(randValue);
               
               


            }
            for (int i = 0; i < hand_size; i++)
            {
                gameHand.xpositions.push_back(800 + 200 * i);
                gameHand.ypositions.push_back(400);
            }
            for (int i = 0; i < gameHand.reka.size(); i++)
            {
                gameHand.reka[i].GetNewImage(0.4);
                std::cout << gameHand.reka[i].GetPath();
            }
            reload_hand = false;
            saved_cards.clear();
        }
        if (confirmButton.isPressed(mousePosition, mousePressed))
        {
            confirm = true;
        }
        if (discardButton.isPressed(mousePosition, mousePressed))
        {
            discard = true;
        }

        for (int i = 0; i < gameHand.reka.size(); i++)
        {
            if (gameHand.reka[i].isPressed(mousePosition, mousePressed))
            {
                
                if (pressed[i] == true)
                {
                    pressed[i] = false;
                }
                else if (pressed[i] == false)
                {
                    pressed[i] = true;
                }

            }
            if (currentscreen == GAMEPLAY && gameHand.xpositions[i] > 200 + 200 * i && move == MOVE)
            {
                gameHand.xpositions[i] -= 4;

                gameHand.reka[i].ChangePosition(gameHand.xpositions[i], gameHand.ypositions[i]);
                if (gameHand.xpositions[i] <= 200 + 200 * i)
                    move = NO_MOVE;
            }
            if (pressed[i] == true && move == NO_MOVE)
            {
                gameHand.xpositions[i] = GetMouseX() - 50;
                gameHand.ypositions[i] = GetMouseY() - 50;
                gameHand.reka[i].ChangePosition(gameHand.xpositions[i], gameHand.ypositions[i]);
            }
            Vector2 point;
            point.x = gameHand.xpositions[i] + 50;
            point.y = gameHand.ypositions[i] + 50;
            bool collision = CheckCollisionPointRec(point, boxA);
            if (collision && !collision_counted[i] && confirm)
            {
                gameHand.reka[i].AssertPoints(points);
                on_table.push_back(gameHand.reka[i]);
                confirm2 = true;
                collision_counted[i] = true;
            }
            else if (!confirm2 && confirm && i == gameHand.reka.size() - 1)
            {
                confirm = false;
            }
            else if (!collision && collision_counted[i])
            {
                collision_counted[i] = false;
            }
            if (collision && discard && discards_left > 0)
            {
                to_delete[i] = true;
                reset = true;

            }
            else if (discard && i == gameHand.reka.size() - 1)
            {
                discard = false;
            }

            if (i == gameHand.reka.size() - 1 && reset)
            {
                int j = 0;
                int h = 0;
                while (j < to_delete.size())
                {
                    if (to_delete[j] == true)
                    {
                        gameHand.reka.erase(gameHand.reka.begin() + j - h);
                        
                        gameHand.xpositions.clear();
                        gameHand.ypositions.clear();
                        pressed.erase(pressed.begin() + j - h);
                        collision_counted.erase(collision_counted.begin() + j - h);
                        to_delete.erase(to_delete.begin() + j - h);
                        reload_hand = true;
                        discard = false;
                        reset = false;
                        move = MOVE;

                    }
                    else if (to_delete[j] == false)
                    {
                        j++;
                    }

                }
                
                discards_left--;
            }
        }
        if (points > 0)
        {
            stan = AssertPremPoints(points, on_table);
            Serve_Extra_Points(power, stan, points, discards_left);
            points_overall += points;
            points2 = points;
            points = 0;
        }
        if (currentscreen == LOST && IsKeyPressed(KEY_ENTER))
        {
            currentscreen = MENU;
        }
       
        if (currentscreen == POWER_UP)
        {
            
            if (losulosu)
            {
                losulosu = false;
                random = GetRandomValue(0, 4);
                random2 = random;
                while (random2 == random)
                {
                    random2 = GetRandomValue(0, 4);
                }
                power[random] = true;
                power[random2] = true;

            }
            
            if (random == 0)
            {
                power1substance = "100+ punktow na poczatku nowej rundy";
            }
            else if (random == 1)
            {
                power1substance = "1.25x punktow w nastepnej rundzie";
            }
            else if (random == 2)
            {
                power1substance = "Trojki daja potrojne punkty";
            }
            else if (random == 3)
            {
                power1substance = "Pary daja podwojne punkty";
            }
            else if (random == 4)
            {
                power1substance = "Kazde pozostale wyrzucenie daje 5 punktow";
            }
            if (random2 == 0)
            {
                power2substance = "100+ punktow na poczatku nowej rundy";
            }
            else if (random2 == 1)
            {
                power2substance = "1.25x punktow w nastepnej rundzie";
            }
            else if (random2 == 2)
            {
                power2substance = "Trojki daja potrojne punkty";
            }
            else if (random2 == 3)
            {
                power2substance = "Pary daja podwojne punkty";
            }
            else if (random2 == 4)
            {
                power2substance = "Kazde pozostale wyrzucenie daje 5 punktow";
            }
            if (powerA.isPressed(mousePosition, mousePressed))
            {
                power[random2] = false;
                currentscreen = GAMEPLAY;
                if (power[0])
                    points_overall = 50 + 50 * round_number;
            }
            if (powerB.isPressed(mousePosition, mousePressed))
            {
                power[random] = false;
                currentscreen = GAMEPLAY;
                if (power[0])
                    points_overall = 50 + 50 * round_number;
            }
        }
        if (points_overall >= round_goal)
        {
            currentscreen = POWER_UP;
            for (int i = 0; i < 5; i++)
            {
                power[i] = false;
            }
            gameDeck.Clear();
            ReloadDeck(gameDeck);
            confirm2 = true;
            losulosu = true;
            round_goal = round_goal * 1.5 + 30;
            round_number++;
            hands_left = 6;
            discards_left = 5;
            if (round_number == 4)
            {
                points_overall_final = points_overall;
                hands_left = 0;
            }
            points_overall = 0;
        }
        if (hands_left == 0 && points_overall < round_goal)
        {
            gameDeck.Clear();
            ReloadDeck(gameDeck);
            confirm2 = true;
            points_overall = 0;
            round_goal = 100;
            currentscreen = LOST;
            if (round_number == 4)
                currentscreen = WON;
            round_number = 1;
            hands_left = 6;
            discards_left = 5;
            stan = "";
            points2 = 0;
           
        }
        if (confirm2)
        {
            gameHand.reka.clear();
            gameHand.xpositions.clear();
            gameHand.ypositions.clear();
            reload_hand = true;
            move = MOVE;
            confirm = false;
            confirm2 = false;
            pressed.clear();
            collision_counted.clear();
            to_delete.clear();
            discard = false;
            reset = false;
            hands_left--;
        }
       
        
        if (currentscreen == GAMEPLAY)
        {


        }
        BeginDrawing();
        if (currentscreen == MENU)
        {
            ClearBackground(BLACK);
            DrawTexture(background, 0, 0, WHITE);
            DrawText(TextFormat("POKER INACZEJ"), 460, 270, 60, LIGHTGRAY);
            startButton.Draw();
            exitButton.Draw();
            leaderboardButton.Draw();
            tutorialButton.Draw();
        }
        else if (currentscreen == GAMEPLAY)
        {
            const char* message = "";
            ClearBackground(BLACK);
            DrawTexture(background, 0, 0, WHITE);
            DrawRectangleRec(boxA, GREEN);
            DrawText(TextFormat("Numer rundy:"), 820, 20, 20, LIGHTGRAY);
            DrawText(TextFormat("%i", round_number), 970, 20, 20, LIGHTGRAY);
            DrawText(TextFormat("Do nastepnej rundy:"), 820, 50, 20, LIGHTGRAY);
            DrawText(TextFormat("%i", round_goal), 1030, 50, 20, LIGHTGRAY);
            DrawText(TextFormat("Zostalo rak:"), 1120, 20, 20, LIGHTGRAY);
            DrawText(TextFormat("%i", hands_left), 1250, 20, 20, LIGHTGRAY);
            DrawText(TextFormat("Zostalo wyrzucen:"), 1120, 50, 20, LIGHTGRAY);
            DrawText(TextFormat("%i", discards_left), 1310, 50, 20, LIGHTGRAY);
            if(stan != "")
                DrawText(TextFormat("W poprzedniej rundzie:"), 1120, 100, 20, LIGHTGRAY);
            if (stan == "REGULAR")
                message = "ZWYKLE ZAGRANIE - 1X MULT";
            if (stan == "PAIR")
                message = "PARA - 1.5X MULT";
            if (stan == "TRIPLE")
                message = "TROJKA - 2.5X MULT";
            if (stan == "TWO PAIRS")
                message = "DWIE PARY - 3X MULT";
            if (stan == "QUADRUPLE")
                message = "CZWORKA - 5X MULT";
            if (stan != "")
            {
                DrawText(TextFormat(message), 1120, 130, 20, LIGHTGRAY);
                DrawText(TextFormat("Przyznano "), 1120, 160, 20, LIGHTGRAY);
                DrawText(TextFormat("%i", points2), 1240, 160, 20, LIGHTGRAY);
                DrawText(TextFormat(" punktow"), 1270, 160, 20, LIGHTGRAY);
            }
            confirmButton.Draw();
            discardButton.Draw();
            for (int i = 0; i < gameHand.reka.size(); i++)
            {
                gameHand.reka[i].Draw();
                //std::cout << gameHand.reka[i].pathh;
            }
            
            DrawText(TextFormat("%i", points_overall), 960, 100, 80, LIGHTGRAY);


        }
        else if (currentscreen == LOST)
        {
            ClearBackground(BLACK);
            DrawTexture(background, 0, 0, WHITE);
            DrawText(TextFormat("Przegrales gre!"), 100, 300, 100, LIGHTGRAY);
            DrawText(TextFormat("Nacisnij klawisz ENTER aby wrocic do menu"), 100, 400, 50, LIGHTGRAY);
        }
        else if (currentscreen == WON)
        {
            ClearBackground(BLACK);
            DrawTexture(background, 0, 0, WHITE);
            DrawText("Wygrales gre - zdobyles", 100, 200, 40, LIGHTGRAY);
            DrawText(TextFormat("%i punktow!", points_overall_final), 100, 250, 40, LIGHTGRAY);
            DrawText("Wpisz swoje imie (tylko litery):", 100, 320, 30, LIGHTGRAY);
            DrawRectangle(100, 360, 300, 40, DARKGRAY);
            DrawText(nameInput, 110, 370, 20, WHITE);
            DrawText("ENTER - Zapisz i pokaz tablice wynikow", 100, 420, 20, LIGHTGRAY);

            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (letterCount < 19)) {
                    nameInput[letterCount++] = (char)key;
                    nameInput[letterCount] = '\0';
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
                letterCount--;
                nameInput[letterCount] = '\0';
            }

            if (IsKeyPressed(KEY_ENTER)) {
                std::string playerName(nameInput);
                HandleWinners(playerName, points_overall_final);
                currentscreen = LEADERBOARD;
            }
        }
        else if (currentscreen == POWER_UP)
        {
            ClearBackground(BLACK);
            DrawTexture(background, 0, 0, WHITE);
            powerA.Draw();
            powerB.Draw();
            DrawText(TextFormat("Wybierz wzmocnienie"), 310, 10, 30, BLACK);
            DrawText(TextFormat(power1substance), 310, 110, 30, BLACK);
            DrawText(TextFormat(power2substance), 310, 360, 30, BLACK);
        }
        else if (currentscreen == LEADERBOARD) {
            ShowLeaderboard();
            if (IsKeyPressed(KEY_ENTER)) {
                currentscreen = MENU;
                letterCount = 0;
                nameInput[0] = '\0';
            }
        }
        else if (currentscreen == TUTORIAL)
        {
            ClearBackground(BLACK);
            DrawTexture(toriel, 0, 0, WHITE);
            if (IsKeyPressed(KEY_ENTER)) {
                currentscreen = MENU;
                letterCount = 0;
                nameInput[0] = '\0';
            }
        }
        
        if (IsKeyDown(KEY_Q))
        {
            DrawText(TextFormat("Opuszczanie..."), 20, 20, 20, LIGHTGRAY);
        }
        EndDrawing();

    }
    CloseWindow();
}