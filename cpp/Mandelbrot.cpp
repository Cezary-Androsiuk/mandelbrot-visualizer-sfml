#include "Mandelbrot.hpp"

#include <cmath>

#include "Real.h"
#include "ColorPalette.hpp"

void computeMandelbrot(cPoint pointLB, cPoint pointRT, cSize size, int iterations, Matrix *mandelbrotMatrix) noexcept
{
    real dX = (pointRT->x - pointLB->x) / (size->width-1);
    real dY = (pointRT->y - pointLB->y) / (size->height-1);
    
    real x0, y0, x, y, xtmp;
    int iteration;
    const long double scale = 256.0 / iterations;

    const real real_4 = 4.0;
    for(int yi=0; yi<size->height; yi++)
    {
        y0 = pointLB->y + yi * dY;
        for(int xi=0; xi<size->width; xi++)
        {
            x0 = pointLB->x + xi * dX;
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
