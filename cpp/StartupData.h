#ifndef STARTUPDATA_H
#define STARTUPDATA_H

#include <cstdio>
#include <fstream>
// #include <numeric>
#include <limits>

#include "Point.h"
#include "Area.h"

#define STARTUP_DATA_FILE "./resources/startupData.txt"

#define DEFAULT_POINT_LB_x   -2
#define DEFAULT_POINT_LB_Y   -1.25
#define DEFAULT_POINT_RT_X   0.5
#define DEFAULT_POINT_RT_Y   1.25
#define DEFAULT_ITERATIONS   256
#define DEFAULT_RESOLUTION   0.8


class StartupData
{
    void ignoreLine(std::ifstream& file, bool gl= true);
    void setDefaultStartupData();
    void verifyLoadedData();
    void loadData();

    StartupData();
    StartupData(const StartupData &) = delete;
    StartupData(StartupData &&) = delete;
    StartupData &operator=(StartupData &) = delete;
    StartupData &operator=(StartupData &&) = delete;

public:
    static StartupData *getInstance();

    Area area;
    int iterations;
    double resolution;
};

#endif // STARTUPDATA_H
