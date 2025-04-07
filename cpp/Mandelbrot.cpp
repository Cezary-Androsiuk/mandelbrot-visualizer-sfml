#include "Mandelbrot.hpp"

void computeMandelbrot(cPoint pointLB, cPoint pointRT, cSize size, int iterations, Matrix *mandelbrotMatrix) noexcept
{
    long double dX = (pointRT->x - pointLB->x) / (size->width-1);
    long double dY = (pointRT->y - pointLB->y) / (size->height-1);
    
    long double x0, y0, x, y, xtmp;
    int iteration;

    for(int yi=0; yi<size->height; yi++)
    {
        y0 = pointLB->y + yi * dY;
        for(int xi=0; xi<size->width; xi++)
        {
            x0 = pointLB->x + xi * dX;
            x = x0;
            y = y0;
            iteration = 0;
            
            while(x*x + y*y <= 4.0 && iteration < iterations)
            {
                xtmp = POW(x) - POW(y) + x0;
                y = 2*x*y + y0;
                x = xtmp;
                iteration++;
            }

            mandelbrotMatrix->setCell(xi, yi, iteration);
        }
    }
}