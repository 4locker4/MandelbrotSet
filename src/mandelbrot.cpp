#include "../includes/mandelbrot.hpp"
#include "../includes/DSL.hpp"

int ClaculateSet (sf::Image * image, MandelbrotData * MandelbrotSizees);

int main ()
{
    WindowCl        MandelbrotWindow = {};
    MandelbrotData  MandelbrotSizes  = {};

    MandelbrotWindow.window.create                (sf::VideoMode (X_WINDOW_SIZE, Y_WINDOW_SIZE), "Mandelbrot");

    MandelbrotWindow.fpsField.setSize             (sf::Vector2f (90.f, 75.f));
    MandelbrotWindow.fpsField.setFillColor        (sf::Color::Black);
    MandelbrotWindow.fpsField.setOutlineColor     (sf::Color::Red);
    MandelbrotWindow.fpsField.setOutlineThickness (1);
    MandelbrotWindow.fpsField.setPosition         (5.f, 5.f);

    MandelbrotWindow.fpsFont.loadFromFile ("includes/TimesNewRoman.ttf");

    MandelbrotWindow.fpsText.setFont          (MandelbrotWindow.fpsFont);
    MandelbrotWindow.fpsText.setCharacterSize (30);
    MandelbrotWindow.fpsText.setColor         (sf::Color::White);
    MandelbrotWindow.fpsText.setPosition      (FPS_FIELD_POSITION_X + 10, FPS_FIELD_POSITION_Y / 2 + 20);

    while (MandelbrotWindow.window.isOpen ())
    {        
        MandelbrotWindow.image.create (X_WINDOW_SIZE, Y_WINDOW_SIZE, sf::Color::White);

        if (MoveLeft)   MandelbrotSizes.shift_x -= dx;
        if (MoveRight)  MandelbrotSizes.shift_x += dx;
        if (MoveDown)   MandelbrotSizes.shift_y += dy;
        if (MoveUp)     MandelbrotSizes.shift_y -= dy;
        if (ScalePlus)  MandelbrotSizes.scale   += dx;
        if (ScaleMinus) MandelbrotSizes.scale   -= dx;

        if (ExitWindow) break;

        CheckToSaveCoords

        MandelbrotWindow.clock.restart ();
        
        ClaculateSet (&MandelbrotWindow.image, &MandelbrotSizes);

        float passed_time = MandelbrotWindow.clock.getElapsedTime().asSeconds();
        float FPS = 1 / passed_time;

        snprintf (FPS_STRING, 9, "FPS: %.f", FPS);
        MandelbrotWindow.fpsText.setString (FPS_STRING);

        MandelbrotWindow.texture.loadFromImage (MandelbrotWindow.image);

        MandelbrotWindow.clock.restart();

        MandelbrotWindow.window.clear    ();
        MandelbrotWindow.window.draw     (MandelbrotWindow.sprite);
        MandelbrotWindow.window.draw     (MandelbrotWindow.fpsField);
        MandelbrotWindow.window.draw     (MandelbrotWindow.fpsText);
        MandelbrotWindow.window.display  ();
    }

    return 0;
}

int ClaculateSet (sf::Image * image, MandelbrotData * MandelbrotSizes)
{
    for (int y_coord = 0; y_coord < Y_WINDOW_SIZE; y_coord++)
    {
        float x0 = ((                - 400.f) * dx + MandelbrotSizes->shift_x - 1.f) * MandelbrotSizes->scale,
              y0 = (((float) y_coord - 300.f) * dy + MandelbrotSizes->shift_y + 0.f) * MandelbrotSizes->scale;

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

            sf::Color color = (N == nMax) ? sf::Color::Black : sf::Color::Green;
        }
    }

    return 0;
}