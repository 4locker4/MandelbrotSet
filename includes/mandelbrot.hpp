#ifndef MANDELBROT_HPP
#define MANDELBROT_HPP

#include <stdio.h>
#include <math.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

typedef struct
{
    float shift_x = 0.f;
    float shift_y = 0.f;
    float scale   = 1.f;
} MandelbrotData;

typedef struct
{
    sf::RenderWindow    window;
    sf::Image           image;
    sf::RectangleShape  fpsField;
    sf::Font            fpsFont;
    sf::Text            fpsText;
    sf::Clock           clock;
    sf::Texture         texture;
    sf::Sprite          sprite;
} WindowCl;

const int   Y_WINDOW_SIZE        = 800, X_WINDOW_SIZE        = 800;
const int   Y_FPS_WINDOW         = 75,  X_FPS_WINDOW         = 100;
const float FPS_FIELD_POSITION_X = 0.f, FPS_FIELD_POSITION_Y = 0.f;

const float dx = 3.f / 800.f,     dy = 3.f / 600.f;
const float ds = 0.9f;

const float r2Max = 10.f;
const int   nMax  = 256;

const int   SIZE_OF_PIXEL_ARRAY  = 8;

char FPS_STRING[9] = {};

typedef struct
{
    float x = 0;
    float y = 0;
} PixelCoords;

PixelCoords pixelArray [SIZE_OF_PIXEL_ARRAY] = {};

void SetColor        (sf::Image * image, int x_coord, int y_coord, int * radiusN);
int  CalculateSet    (sf::Image * image, MandelbrotData * MandelbrotSizees);
int  StartRendering  ();



#endif // MANDELBROT_HPP