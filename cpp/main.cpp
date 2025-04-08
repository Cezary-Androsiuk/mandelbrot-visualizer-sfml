#include <cstdio>
#include <cstdlib>
#include <assert.h>

#include <string>

#include "StartupData.h"
#include "Program.hpp"


// mkdir build
// cd build
// cmake -G "MinGW Makefiles" .. 

// mingw32-make -j4
// main.exe

// mingw32-make -j4 && main.exe


int main(int argc, char **argv)
{
    Program::displaySFML(StartupData::getInstance());
}
