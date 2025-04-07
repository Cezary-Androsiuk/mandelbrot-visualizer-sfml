#include <cstdio>
#include <cstdlib>
#include <assert.h>

#include <string>

#include "Program.hpp"

// mkdir build
// cd build
// cmake -G "MinGW Makefiles" .. 

// mingw32-make -j4
// main.exe -2 -1.25 0.5 1.25 256 1.

// mingw32-make -j4 && main.exe -2 -1.25 0.5 1.25 256 1.


int main(int argc, char **argv)
{
    if(argc != 7)
    {
        printf("to few argruments, 5 are required\n");
        printf(
            "pointLB.x (float)\n"
            "pointLB.y (float)\n"
            "pointRT.x (float)\n"
            "pointRT.y (float)\n"
            "iterations (int)\n"
            "resolution (float)\n"
            );
        return 1;
    }
    
    // 1080/1920 = 0.5625

    const Point pointLB = {atof(argv[1]), atof(argv[2])};
    const Point pointRT = {atof(argv[3]), atof(argv[4])};
    const int iterations = atoi(argv[5]);
    const long double resolution = atof(argv[6]);

    assert(pointLB.x < pointRT.x && "Left Point should be the left one");
    assert(pointLB.y < pointRT.y && "Bottom Point should be the bottom one");
    
    Program::displaySFML(&pointLB, &pointRT, iterations, resolution);
}