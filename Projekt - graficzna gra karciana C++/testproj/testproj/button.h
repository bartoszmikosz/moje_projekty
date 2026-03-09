#ifndef BUTTON_H
#define BUTTON_H


#include <iostream>
#include <raylib.h>


class Button
{
public:
    Button() {};
    Button(const char* imagePath, Vector2 imagePosition, float scale);
    ~Button();
    void Draw();
    bool isPressed(Vector2 mousePos, bool mousePressed);
    void ChangePosition(int number, int number2);
    void SetTexture(Texture2D text);
    Texture2D texture;
    const char* pathh;
private:

    Vector2 position;
};

class Card : public Button
{
public:
    Card(const char* imagePath, Vector2 imagePosition, float scale, int points_new) : Button(imagePath, imagePosition, scale) {
        points = points_new;
    };
    Card(const char* imagePath, float scale, int points_new);
    void GetNewImage(float scale);
    void AssertPoints(int& number);
    void SetPoints(int number);
    int GetPoints();
    void SetPath(std::string pathh);
    std::string GetPath();

private:
    int points;
    std::string path;


};

#endif