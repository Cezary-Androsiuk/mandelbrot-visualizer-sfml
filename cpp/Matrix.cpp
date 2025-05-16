#include "Matrix.hpp"

#include "Support.h"
#include <fstream>
#include <iomanip> // set width of int while saving

const sf::Color Matrix::blackColor = sf::Color::Black;

Matrix::Matrix(const Size *size)
    : m_matrixSize{size->field()}
    , m_matrix{new int[m_matrixSize]}
    , m_colorMatrix{new sf::Color[m_matrixSize]}
    , m_size{size->width, size->height}
{

}

Matrix::~Matrix(){

}

bool Matrix::cellExist(int index) const noexcept
{
#if SAFE_MODE
    if(UNLIKELY(index >= m_matrixSize))
    {
        int x = index % m_size.width;
        int y = index / m_size.width;
        fprintf(stderr, "cannot set value to x=%d and y=%d\n", x, y);
        fprintf(stdout, "matrix size is x=%d and y=%d\n", m_size.width, m_size.height);
        return false;
    }
#endif
    return true;
}

void Matrix::setCell(int x, int y, uint8_t value) noexcept
{
    int index = x + y*m_size.width;
    if(!this->cellExist(index))
        return;

    m_matrix[index] = value;
}

uint8_t Matrix::getCell(int x, int y) const noexcept
{
    int index = x + y*m_size.width;
    if(!this->cellExist(index))
        return 0;

    return m_matrix[index];
}

void Matrix::setColorCell(int x, int y, const sf::Color &value) noexcept
{
    int index = x + y*m_size.width;
    if(UNLIKELY(!this->cellExist(index)))
        return;

    m_colorMatrix[index] = value;
}

const sf::Color &Matrix::getColorCell(int x, int y) const noexcept
{
    int index = x + y*m_size.width;
    if(UNLIKELY(!this->cellExist(index)))
        return Matrix::blackColor;

    return m_colorMatrix[index];
}

void Matrix::saveToFile(std::string fileName)
{
    std::ofstream file(fileName);
    if(!file.good())
    {
        fprintf(stderr, "cannot save matrix to file\n");
        return;
    }

    const size_t lastYIndex = m_size.height -1;
    const size_t lastXIndex = m_size.width -1;
    for(int yi=0; yi<m_size.height; yi++)
    {
        // vuint8 &row = *m_matrix[yi];
        for(int xi=0; xi<m_size.width; xi++)
        {
            int index = xi + yi * m_size.width;
            file << std::setw(3) << (int)m_matrix[index] << (xi != lastXIndex ? " " : "");
        }

        if(yi != lastYIndex)
            file << std::endl;
    }

    file.close();
}

const Size &Matrix::getSize() const
{
    return m_size;
}
