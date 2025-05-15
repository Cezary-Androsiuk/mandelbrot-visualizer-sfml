#include "Matrix.hpp"

Matrix::Matrix(const Size *size)
    : m_matrix{ (size_t) size->height }
    , m_size{size->width, size->height}
{
    for(int i=0; i<m_matrix.size(); i++)
        m_matrix[i] = new vuint8(size->width, 0u);
}

Matrix::~Matrix(){
    for(vuint8 *row : m_matrix)
        delete row;
}

void Matrix::setCell(int x, int y, uint8_t value) noexcept
{
    // if(y >= m_matrix.size())
    // {
    //     fprintf(stderr, "cannot set value %f to row y=%d\n", y);
    //     return;
    // }

    // vuint8 &row = *m_matrix[y];
    // if(x >= row.size())
    // {
    //     fprintf(stderr, "cannot set value %f to point (x=%d, y=%d)\n", x, y);
    //     return;
    // }

    // row[x] = value;

    (*m_matrix[y])[x] = value;
}

uint8_t Matrix::getCell(int x, int y) const noexcept
{
    // if(y >= m_matrix.size())
    // {
    //     fprintf(stderr, "cannot set value %f to row y=%d\n", y);
    //     return 0;
    // }

    // vuint8 &row = *m_matrix[y];
    // if(x >= row.size())
    // {
    //     fprintf(stderr, "cannot set value %f to point (x=%d, y=%d)\n", x, y);
    //     return 0;
    // }

    // return row[x]; /// ~37ms in usage
    // return m_matrix.at(y)->at(x); /// ~47ms in usage
    return (*m_matrix[y])[x]; /// ~25ms in usage
    // return 0; /// ~1ms in usage
}

void Matrix::saveToFile(std::string fileName)
{
    std::ofstream file(fileName);
    if(!file.good())
    {
        fprintf(stderr, "cannot save matrix to file\n");
        return;
    }

    size_t lastYIndex = m_matrix.size() -1;
    for(int yi=0; yi<m_matrix.size(); yi++)
    {
        vuint8 &row = *m_matrix[yi];
        size_t lastXIndex = row.size() -1;
        for(int xi=0; xi<row.size(); xi++)
        {
            file << std::setw(3) << (int)row[xi] << (xi != lastXIndex ? " " : "");
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
