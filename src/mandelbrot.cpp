#include "../includes/mandelbrot.hpp"
#include "../includes/DSL.hpp"
#include <mmintrin.h>

inline int CalculateRadius (float x, float y, float x0, float y0);

#define GRAPHIC

int main ()
{
    StartRendering ();

    return 0;
}

int StartRendering ()
{
    MandelbrotData  MandelbrotSizes  = {};                                              // Init shifts and scale

    sf::Clock           clock;

#ifdef GRAPHIC

    sf::RenderWindow    window;
    sf::Image           image;
    sf::RectangleShape  fpsField;
    sf::Font            fpsFont;
    sf::Text            fpsText;
    sf::Texture         texture;

    window.create                (sf::VideoMode (X_WINDOW_SIZE, Y_WINDOW_SIZE), "Mandelbrot");

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//                                      FPS                                     
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

    fpsField.setSize             (sf::Vector2f (90.f, 75.f));          // Init FPS`s field
    fpsField.setFillColor        (sf::Color::Black);
    fpsField.setOutlineColor     (sf::Color::Red);
    fpsField.setOutlineThickness (1);
    fpsField.setPosition         (5.f, 5.f);

    fpsFont.loadFromFile ("includes/TimesNewRoman.ttf");               // Get font 

    fpsText.setFont          (fpsFont);                                // Init FPS text
    fpsText.setCharacterSize (30);
    fpsText.setColor         (sf::Color::White);
    fpsText.setPosition      (FPS_FIELD_POSITION_X + 10, FPS_FIELD_POSITION_Y / 2 + 20);

#endif // GRAPHIC

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//                                  Start
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

    while (true)
    {

#ifdef GRAPHIC

        image.create (X_WINDOW_SIZE, Y_WINDOW_SIZE, sf::Color::White);

//======   Shifts   ====================================================================
        if (MoveLeft)   MandelbrotSizes.shift_x -= dx / MandelbrotSizes.scale;
        if (MoveRight)  MandelbrotSizes.shift_x += dx / MandelbrotSizes.scale;
        if (MoveDown)   MandelbrotSizes.shift_y += dy / MandelbrotSizes.scale;
        if (MoveUp)     MandelbrotSizes.shift_y -= dy / MandelbrotSizes.scale;
        if (ScalePlus)  MandelbrotSizes.scale   *= ds;
        if (ScaleMinus) MandelbrotSizes.scale   /= ds;

        if (ExitWindow) break;                                  // Break

        CheckToSaveCoords                                       // Check if user wants to save shifts
//======   ------   ====================================================================

#endif // GRAPHIC

#ifndef GRAPHIC

        sf::Image image;

#endif // GRAPHIC

        clock.restart ();                      // Start FPS
        
        CalculateSet (&image, &MandelbrotSizes);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//                                  Take FPS
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

        float passed_time = clock.getElapsedTime().asSeconds();
        float FPS = 1 / passed_time;

#ifndef GRAPHIC

        printf ("FPS:   %2.f\n", FPS);

#endif // GRAPHIC

#ifdef GRAPHIC

        snprintf (FPS_STRING, 9, "FPS: %.f", FPS);
        fpsText.setString (FPS_STRING);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

        texture.loadFromImage (image);    // Render image
        sf::Sprite sprite (texture);

        window.clear    ();
        window.draw     (sprite);
        window.draw     (fpsField);
        window.draw     (fpsText);
        window.display  ();

#endif // GRAPHIC
    }

    return 0;
}

int CalculateSet (sf::Image * image, MandelbrotData * MandelbrotSizes)
{
    for (int y_coord = 0; y_coord < Y_WINDOW_SIZE; y_coord++)
    {
        float x0 = ((                - 400.f) * dx + MandelbrotSizes->shift_x - 1.f) * MandelbrotSizes->scale,
              y0 = (((float) y_coord - 400.f) * dy + MandelbrotSizes->shift_y + 0.f) * MandelbrotSizes->scale;

        for (int x_coord = 0; x_coord < X_WINDOW_SIZE; x_coord += SIZE_OF_PIXEL_ARRAY, x0 += 4 * dx)
        {
            float x_base  [SIZE_OF_PIXEL_ARRAY] = {x0         ,
                                                   x0 + dx    ,
                                                   x0 + dx * 2,
                                                   x0 + dx * 3};

            int   radiosN [SIZE_OF_PIXEL_ARRAY] = {0};
            
            for (int i = 0; i < SIZE_OF_PIXEL_ARRAY; i++)
            {
                radiosN[i] = CalculateRadius (x_base[i], y0, x0, y0);
            }

#ifdef GRAPHIC

            SetColor (image, x_coord, y_coord, radiosN);

#endif // GRAPHIC
        }
    }

    return 0;
}

void SetColor (sf::Image * image, int x_coord, int y_coord, int * radiusN)
{
    sf::Color color;

    for (int i = 0; i < SIZE_OF_PIXEL_ARRAY; i++)
    {
        color = (radiusN[i] == nMax) ? sf::Color::Black : sf::Color ((uint8_t) radiusN[i] * 18 % 255, 
                                                                     (uint8_t) radiusN[i] * 10 % 255, 
                                                                     (uint8_t) radiusN[i] * 15 % 255);

        image->setPixel (x_coord + i, y_coord, color);
    }

    return;
}

inline int CalculateRadius (float x, float y, float x0, float y0)
{
    int N = 0;

    for ( ; N < nMax; N++)
    {
        float x2 = x * x,
              y2 = y * y,
              xy = x * y;

        float r2 = x2 + y2;

        if (r2 >= r2Max) return N;

        x = x2 - y2 + x0;
        y = 2 * xy + y0;
    }

    return N;
}