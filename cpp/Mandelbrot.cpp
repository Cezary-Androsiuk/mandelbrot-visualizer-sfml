#include "Mandelbrot.hpp"

#include <cmath>

#include "Real.h"
#include "ColorPalette.hpp"

void computeMandelbrot(real l, real b, real r, real t, int width, int height,
                       int iterations, Matrix *mandelbrotMatrix) noexcept
{
    real dX = (r - l) / (width-1);
    real dY = (t -b) / (height-1);

    real x0, y0, x, y, xtmp;
    int iteration;
    const long double scale = 256.0 / iterations;

    const real real_4 = 4.0;
    for(int yi=0; yi<height; yi++)
    {
        y0 = b + yi * dY;
        for(int xi=0; xi<width; xi++)
        {
            x0 = l + xi * dX;
            x = x0;
            y = y0;
            iteration = 0;

            while(x*x + y*y <= real_4 && iteration < iterations)
            {
                xtmp = POW(x) - POW(y) + x0;
                y = 2*x*y + y0;
                x = xtmp;
                iteration++;
            }

            // mandelbrotMatrix->setCell(xi, yi, iteration);

            int colorIndex = (int) floor(5.0 * scale * log2f(1.0f * iteration + 1));

            sf::Color pixelColor;
            pixelColor.r = ColorPalette[colorIndex][0];
            pixelColor.g = ColorPalette[colorIndex][2];
            pixelColor.b = ColorPalette[colorIndex][1];

            mandelbrotMatrix->setColorCell(xi, yi, pixelColor);

        }
    }
}

void computeMandelbrotPart(int threadID,
                           real l, real b, real r, real t, int width, int height,
                           int heightOffset, int iterations, Matrix *mandelbrotMatrix) noexcept
{
    real dX = (r - l) / (width-1);
    real dY = (t -b) / (height-1);

    real x0, y0, x, y, xtmp;
    int iteration;
    const long double scale = 256.0 / iterations;

    const real real_4 = 4.0;
    for(int yi=0; yi<height; yi++)
    {
        y0 = b + yi * dY;
        for(int xi=0; xi<width; xi++)
        {
            x0 = l + xi * dX;
            x = x0;
            y = y0;
            iteration = 0;

            while(x*x + y*y <= real_4 && iteration < iterations)
            {
                xtmp = POW(x) - POW(y) + x0;
                y = 2*x*y + y0;
                x = xtmp;
                iteration++;
            }

            // mandelbrotMatrix->setCell(xi, yi, iteration);

            int colorIndex = (int) floor(5.0 * scale * log2f(1.0f * iteration + 1));

            sf::Color pixelColor;
            if(threadID % 3 == 0)
                pixelColor.r = ColorPalette[colorIndex][0];
            else if (threadID % 3 == 1)
                pixelColor.g = ColorPalette[colorIndex][2];
            else if (threadID % 3 == 2)
                pixelColor.b = ColorPalette[colorIndex][1];

            mandelbrotMatrix->setColorCell(xi, yi+heightOffset, pixelColor);

        }
    }
}
