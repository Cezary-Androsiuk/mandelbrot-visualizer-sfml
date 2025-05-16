#ifndef MANDELBROT_HPP
#define MANDELBROT_HPP

#include "Point.h"
#include "Size.h"
#include "Matrix.hpp"

struct MBdata{
    cPoint pointLB;
    cPoint pointRT;
    cSize size;
    int iterations;
    Matrix *mandelbrotMatrix;
};

void computeMandelbrot(
    real l, real b, real r, real t, int width, int height,
    int iterations, Matrix *mandelbrotMatrix) noexcept;

void computeMandelbrotPart(int threadID,
    real l, real b, real r, real t, int width, int height,
    int heightOffset, int iterations, Matrix *mandelbrotMatrix) noexcept;

#endif // MANDELBROT_HPP
