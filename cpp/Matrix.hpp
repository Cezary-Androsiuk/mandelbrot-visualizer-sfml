#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip> // set width of int while saving

#include "Point.h"
#include "Size.h"

// #include "SFML/Graphics.hpp"

typedef std::vector<uint8_t> vuint8;

#define POW(x) ((x) * (x))

class Matrix{
public:
    Matrix(const Size *size);
    ~Matrix();

    void setCell(int x, int y, uint8_t value) noexcept;
    uint8_t getCell(int x, int y) const noexcept;
    void saveToFile(std::string fileName);
    const Size &getSize() const;

private:
    vuint8 m_matrix;
    Size m_size;
};

#endif // MATRIX_HPP
