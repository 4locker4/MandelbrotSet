#include "../includes/mandelbrot.hpp"
#include "../includes/DSL.hpp"
#include <immintrin.h>

inline int CalculateRadius (float x, float y, float x0, float y0);

#define GRAPHIC
#define MUL_X0(i) x0 + dx * i

const __m256 r2Max_array = _mm256_set1_ps (r2Max);

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
        float x0 = ((                - 400.f * MandelbrotSizes->scale) * dx + MandelbrotSizes->shift_x - 1.f) * MandelbrotSizes->scale,
              y0 = (((float) y_coord - 400.f * MandelbrotSizes->scale) * dy + MandelbrotSizes->shift_y + 0.f) * MandelbrotSizes->scale;

        for (int x_coord = 0; x_coord < X_WINDOW_SIZE; x_coord += SIZE_OF_PIXEL_ARRAY, x0 += SIZE_OF_PIXEL_ARRAY * dx * MandelbrotSizes->scale)
        {
            __m256 x_0_array = _mm256_add_ps (_mm256_set1_ps (x0),
                                              _mm256_mul_ps  (_mm256_set_ps (7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f),
                                              _mm256_set1_ps (dx * MandelbrotSizes->scale)));

            __m256 y_0_array = _mm256_set1_ps (y0);

            __m256 x = x_0_array;
            __m256 y = y_0_array;

            __m256i n_array = _mm256_setzero_si256 ();
            
            for (int n = 0; n < nMax; n++)
            {
                __m256 x2 = _mm256_mul_ps (x, x);
                __m256 y2 = _mm256_mul_ps (y, y);
                __m256 xy = _mm256_mul_ps (x, y);

                __m256 r2 = _mm256_add_ps (x2, y2);

                __m256 compareRes = _mm256_cmp_ps (r2, r2Max_array, _CMP_LE_OQ);

                int check = _mm256_movemask_ps (compareRes);

                if (!check) break;

                x = _mm256_sub_ps (x2, _mm256_add_ps (y2, x_0_array));   
                y = _mm256_add_ps (y_0_array, _mm256_mul_ps (xy, _mm256_set1_ps (2.f)));

                n_array = _mm256_add_epi32 (n_array, _mm256_castps_si256 (compareRes));
            }

#ifdef GRAPHIC

            int radiusN[8] = {0};

            _mm256_storeu_si256 ((__m256i*) radiusN, n_array);

            SetColor (image, x_coord, y_coord, radiusN);

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