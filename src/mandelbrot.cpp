#include "../includes/mandelbrot.h"

const int Y_WINDOW_SIZE = 600, X_WINDOW_SIZE = 800;

const float dx = 1/800, dy = 1/800;

const float ROI_X = 0, ROI_Y = 0;

int main ()
{
    sf::RenderWindow window (sf::VideoMode(X_WINDOW_SIZE, Y_WINDOW_SIZE), "Mandelbrot");

    sf::Event event;

    sf::Keyboard keyboard;

    float shift_x = 0, shift_y = 0, scale = 0;
    int nMax = 256;

    while (window.isOpen ())
    {
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Left  ))  shift_x += dx;
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Right ))  shift_x -= dx;
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Up    ))  shift_y += dy;
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Down  ))  shift_y -= dy;
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Equal ))  scale   += dx;
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Dash  ))  scale   -= dx;

        for (int y_coord = 0; y_coord < Y_WINDOW_SIZE; y_coord++)
        {
            if (sf::Keyboard::isKeyPressed (sf::Keyboard::Q)) break;

            if (sf::Keyboard::isKeyPressed (sf::Keyboard::LControl))
                if (sf::Keyboard::isKeyPressed (sf::Keyboard::S))
                {
                    ; // Create .txt with coords
                }
    
            float x0 = ((                - 400.f) * dx + ROI_X + shift_x),
                  y0 = (((float) y_coord - 300.f) * dy + ROI_Y + shift_y);
    
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

                    x = x2 - y2 + x0;
                    y = y2 + xy + y0;
                }
            
                float I = sqrtf (sqrtf ((float) N / (float) nMax)) * 255.f;                
            }
        }

        window.clear ();
        window.display ();
    }
}