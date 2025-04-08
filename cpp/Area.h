#ifndef AREA_H
#define AREA_H

#include "Point.h"

struct Area
{
    Point leftBottom;
    Point rightTop;
};
typedef const Area *cArea;

#endif // AREA_H
