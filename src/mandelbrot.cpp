#include "../includes/mandelbrot.hpp"
#include "../includes/DSL.hpp"

int ClaculateSet (sf::Image * image, MandelbrotData * MandelbrotSizees);

int main ()
{
    MandelbrotData  MandelbrotSizes  = {};                                              // Init shifts and scale

    sf::RenderWindow    window;
    sf::Image           image;
    sf::RectangleShape  fpsField;
    sf::Font            fpsFont;
    sf::Text            fpsText;
    sf::Clock           clock;
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


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//                                  Start
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

    while (window.isOpen ())
    {        
        image.create (X_WINDOW_SIZE, Y_WINDOW_SIZE, sf::Color::White);

//======   Shifts   ====================================================================
        if (MoveLeft)   MandelbrotSizes.shift_x -= dx;
        if (MoveRight)  MandelbrotSizes.shift_x += dx;
        if (MoveDown)   MandelbrotSizes.shift_y += dy;
        if (MoveUp)     MandelbrotSizes.shift_y -= dy;
        if (ScalePlus)  MandelbrotSizes.scale   += dx;
        if (ScaleMinus) MandelbrotSizes.scale   -= dx;

        if (ExitWindow) break;                                  // Break

        CheckToSaveCoords                                       // Check if user wants to save shifts
//======   ------   ====================================================================

        clock.restart ();                      // Start FPS
        
        ClaculateSet (&image, &MandelbrotSizes);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//                                  Take FPS
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

        float passed_time = clock.getElapsedTime().asSeconds();
        float FPS = 1 / passed_time;

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
    }

    return 0;
}

int ClaculateSet (sf::Image * image, MandelbrotData * MandelbrotSizes)
{
    for (int y_coord = 0; y_coord < Y_WINDOW_SIZE; y_coord++)
    {
        float x0 = ((                - 400.f) * dx + MandelbrotSizes->shift_x - 1.f) * MandelbrotSizes->scale,
              y0 = (((float) y_coord - 400.f) * dy + MandelbrotSizes->shift_y + 0.f) * MandelbrotSizes->scale;

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
                y = 2 * xy + y0;
            }

            sf::Color color = (N == nMax) ? sf::Color::Black : sf::Color ((uint8_t) sqrt (255 - N * 0.8), (uint8_t) sqrt (N * 3), (uint8_t) 255 - 255 % N);

            image->setPixel (x_coord, y_coord, color);
        }
    }

    return 0;
}