#include "JAProjektDllCPlus.h"
#include <iostream>

/*
 * Funkcja: ModifyPixel2 (Implementacja C++)
 * Nak³ada filtr sepii na wiersz obrazu w formacie BGRA (32-bit).
 * Dzia³anie: Przetwarza piksele sekwencyjnie (jeden po drugim), wykorzystuj¹c
 * standardowe obliczenia zmiennoprzecinkowe. Ka¿da sk³adowa koloru
 * jest wyliczana jako suma wa¿ona sk³adowych RGB, a nastêpnie w
 *kazdym kroku sprawdzane jest nasycenie (limit do 255).
 */

extern "C" __declspec(dllexport)
void ModifyPixel2(unsigned char* rowPtr, int width)
{
    
    for (int x = 0; x < width; ++x)
    {
        int bytesPerPixel = 4;
        unsigned char* pixel = rowPtr + x * bytesPerPixel;

        unsigned char blue = pixel[0];
        unsigned char green = pixel[1];
        unsigned char red = pixel[2];

        int newRed = (int)(0.393 * red + 0.769 * green + 0.189 * blue);
        int newGreen = (int)(0.349 * red + 0.686 * green + 0.168 * blue);
        int newBlue = (int)(0.272 * red + 0.534 * green + 0.131 * blue);

        if (newRed > 255) newRed = 255;
        if (newGreen > 255) newGreen = 255;
        if (newBlue > 255) newBlue = 255;

        pixel[0] = (unsigned char)newBlue;
        pixel[1] = (unsigned char)newGreen;
        pixel[2] = (unsigned char)newRed;
    }
}
