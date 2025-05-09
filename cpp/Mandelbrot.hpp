#ifndef MANDELBROT_HPP
#define MANDELBROT_HPP

#include "Matrix.hpp"

void computeMandelbrot(cPoint pointLB, cPoint pointRT, cSize size, int iterations, Matrix *mandelbrotMatrix) noexcept;

#endif // MANDELBROT_HPP