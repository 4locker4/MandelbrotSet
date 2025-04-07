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

const int   Y_WINDOW_SIZE        = 600, X_WINDOW_SIZE        = 800;
const int   Y_FPS_WINDOW         = 75,  X_FPS_WINDOW         = 100;
const float FPS_FIELD_POSITION_X = 0.f, FPS_FIELD_POSITION_Y = 0.f;

const float dx = 1.f / 800.f,     dy = 1.f / 600.f;

const float r2Max = 10.f;
const int   nMax  = 256;

char FPS_STRING[9] = {};

#endif // MANDELBROT_HPP