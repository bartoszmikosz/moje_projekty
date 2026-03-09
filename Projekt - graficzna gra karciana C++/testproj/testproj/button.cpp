
#include <iostream>
#include <raylib.h>
#include <string>

#include "button.h"

Button::Button(const char* imagePath, Vector2 imagePosition, float scale)
{
    //const char* pathh = imagePath.c_str();
    Image image = LoadImage(imagePath);

    int originalWidth = image.width;
    int originalHeight = image.height;

    int newWidth = static_cast<int>(originalWidth * scale);
    int newHeight = static_cast<int>(originalHeight * scale);

    ImageResize(&image, newWidth, newHeight);
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    position = imagePosition;
}


Button::~Button()
{
    UnloadTexture(texture);
}

void Button::Draw()
{
    DrawTextureV(texture, position, WHITE);
    //std::cout << pathh;
}

bool Button::isPressed(Vector2 mousePos, bool mousePressed)
{
    Rectangle rect = { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };

    if (CheckCollisionPointRec(mousePos, rect) && mousePressed)
    {
        return true;
    }
    return false;
}

void Button::SetTexture(Texture2D text)
{
    texture = text;
}


Card::Card(const char* imagePath, float scale, int points_new)
{
    path = imagePath;
    Texture2D textur;
    //const char* pathh = imagePath.c_str();
    Image image = LoadImage(imagePath);

    int originalWidth = image.width;
    int originalHeight = image.height;

    int newWidth = static_cast<int>(originalWidth * scale);
    int newHeight = static_cast<int>(originalHeight * scale);

    ImageResize(&image, newWidth, newHeight);
    textur = LoadTextureFromImage(image);
    SetTexture(textur);
    UnloadImage(image);
    points = points_new;
}

void Card::GetNewImage(float scale)
{
    //UnloadTexture(texture);
    pathh = path.c_str();
    //Image image = LoadImage(pathh);
    /*Texture2D textur;
    Image image = LoadImage(imagePath);

    int originalWidth = image.width;
    int originalHeight = image.height;

    int newWidth = static_cast<int>(originalWidth * scale);
    int newHeight = static_cast<int>(originalHeight * scale);

    ImageResize(&image, newWidth, newHeight);
    textur = LoadTextureFromImage(image);
    SetTexture(textur);
    UnloadImage(image);*/
    Image image = LoadImage(pathh);

    int originalWidth = image.width;
    int originalHeight = image.height;

    int newWidth = static_cast<int>(originalWidth * scale);
    int newHeight = static_cast<int>(originalHeight * scale);

    ImageResize(&image, newWidth, newHeight);
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    //position = imagePosition;

}

void Card::SetPoints(int number)
{
    points = number;
}

void Card::AssertPoints(int& number)
{
    number += points;
}

void Card::SetPath(std::string pathh)
{
    path = pathh;
}

std::string Card::GetPath()
{
    return path;
}

int Card::GetPoints()
{
    return points;
}

void Button::ChangePosition(int number, int number2)
{
    position.x = number;
    position.y = number2;
}
