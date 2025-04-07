#include "../includes/mandelbrot.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

int ClaculateSet (sf::Image * image, float shift_x, float shift_y, float scale);

const int Y_WINDOW_SIZE = 600, X_WINDOW_SIZE = 800;
const int Y_FPS_WINDOW  = 75, X_FPS_WINDOW  = 100;
const float FPS_FIELD_POSITION_X = 0.f, FPS_FIELD_POSITION_Y = 0.f;

const float dx = 1/ X_WINDOW_SIZE, dy = 1 / X_WINDOW_SIZE;

const float r2Max = 100.f;
const int   nMax  = 256;

char FPS_STRING[9] = {};

int main ()
{
    sf::RenderWindow window (sf::VideoMode (X_WINDOW_SIZE, Y_WINDOW_SIZE), "Mandelbrot");

    sf::Event event;

    sf::Keyboard keyboard;

    sf::Image   image;
    sf::Texture texture;
    sf::Sprite  sprite;

    sf::RectangleShape fpsField (sf::Vector2f (90.f, 75.f));

    fpsField.setFillColor        (sf::Color::Black);
    fpsField.setOutlineColor     (sf::Color::Red);
    fpsField.setOutlineThickness (1);
    fpsField.setPosition         (5.f, 5.f);

    sf::Font fpsFont;

    fpsFont.loadFromFile ("includes/TimesNewRoman.ttf");

    sf::Text fpsText;

    fpsText.setFont (fpsFont);
    fpsText.setCharacterSize (30);
    fpsText.setColor (sf::Color::White);
    fpsText.setPosition (FPS_FIELD_POSITION_X + 10, FPS_FIELD_POSITION_Y / 2 + 20);

    sf::Clock clock;
    
    float shift_x = 0, shift_y = 0, scale = 1;

    while (window.isOpen ())
    {
        image.create (X_WINDOW_SIZE, Y_WINDOW_SIZE, sf::Color::White);
        
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Left  ))  shift_x += dx;
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Right ))  shift_x -= dx;
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Up    ))  shift_y += dy;
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Down  ))  shift_y -= dy;
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Equal ))  scale   += dx;
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Dash  ))  scale   -= dx;

        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Q)) break;

        if (sf::Keyboard::isKeyPressed (sf::Keyboard::LControl))
            if (sf::Keyboard::isKeyPressed (sf::Keyboard::S))
            {
                ; // Create .txt with coords
            }

        clock.restart ();
        
        ClaculateSet (&image, shift_x, shift_y, scale);

        float passed_time = clock.getElapsedTime().asSeconds();
        float FPS = 1 / passed_time;

        snprintf (FPS_STRING, 9, "FPS: %.f", FPS);
        fpsText.setString (FPS_STRING);
        
        texture.loadFromImage (image);
        sprite.setTexture     (texture);

        clock.restart();

        window.clear    ();
        window.draw     (sprite);
        window.draw     (fpsField);
        window.draw     (fpsText);
        window.display  ();

        printf ("draw\n");
    }

    return 0;
}

int ClaculateSet (sf::Image * image, float shift_x, float shift_y, float scale)
{
    for (int y_coord = 0; y_coord < Y_WINDOW_SIZE; y_coord++)
    {
        float x0 = ((                - X_WINDOW_SIZE / 2) * dx + shift_x) * scale,
              y0 = (((float) y_coord - Y_WINDOW_SIZE / 2) * dy + shift_y) * scale;

        for (int x_coord = 0; x_coord < X_WINDOW_SIZE; x_coord++, x0 += dx)
        {
            float x = x0,
                  y = y0;

            int N = 0;

            for ( ; N < nMax; N++)
            {
                float x2 = x * x,
                      y2 = y * y,
                      xy = x * y;                    

                float r2 = x2 + y2;

                if (r2 >= r2Max) break;

                x = x2 - y2 + x0;
                y = y2 + xy + y0;
            }
        
            float I = sqrtf (sqrtf ((float) N / (float) nMax)) * 255.f;

            sf::Color color;

            color = (N < nMax) ? sf::Color ((uint8_t) (I), (uint8_t) (32 % N), (uint8_t) I, (uint8_t) (255 - 255 % N))
                               : sf::Color ();

            image->setPixel (x_coord, y_coord, color);
        }
    }

    return 0;
}