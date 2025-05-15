#ifndef SIZE_H
#define SIZE_H

struct Size
{
    int width;
    int height;

    int field() const;
};
typedef const Size *cSize;

#endif // SIZE_H
