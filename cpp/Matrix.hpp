#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <string>

// #include "Point.h"
#include "Size.h"

#include "SFML/Graphics/Color.hpp"

#define POW(x) ((x) * (x))

class Matrix{
public:
    Matrix(const Size *size);
    ~Matrix();

    inline bool cellExist(int index) const noexcept;

    void setCell(int x, int y, uint8_t value) noexcept;
    uint8_t getCell(int x, int y) const noexcept;

    void setColorCell(int x, int y, const sf::Color &value) noexcept;
    const sf::Color &getColorCell(int x, int y) const noexcept;

    void saveToFile(std::string fileName);
    const Size &getSize() const;

private:
    const int m_matrixSize;
    Size m_size;
    int *m_matrix;
    sf::Color *m_colorMatrix;
    static const sf::Color blackColor;
};

#endif // MATRIX_HPP
