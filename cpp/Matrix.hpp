#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip> // set width of int while saving

typedef std::vector<uint8_t> vuint8;
typedef std::vector<vuint8*> vvuint8;

#define POW(x) ((x) * (x))

struct Point{
    long double x;
    long double y;
};
typedef const Point *cPoint;

struct Size{
    int width;
    int height;
};
typedef const Size *cSize;

class Matrix{
public:
    Matrix(const Size *size);
    ~Matrix();

    void setCell(int x, int y, uint8_t value) noexcept;
    uint8_t getCell(int x, int y) const noexcept;
    void saveToFile(std::string fileName);
    const Size &getSize() const;

private:
    vvuint8 m_matrix;
    Size m_size;
};

#endif // MATRIX_HPP