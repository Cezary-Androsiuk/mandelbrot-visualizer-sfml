#include "StartupData.h"

void StartupData::ignoreLine(std::ifstream& file, bool gl) {
    if(gl)
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


void StartupData::setDefaultStartupData()
{
    this->pointLB.x = DEFAULT_POINT_LB_x;
    this->pointLB.y = DEFAULT_POINT_LB_Y;

    this->pointRT.x = DEFAULT_POINT_RT_X;
    this->pointRT.y = DEFAULT_POINT_RT_Y;

    this->iterations = DEFAULT_ITERATIONS;
    this->resolution = DEFAULT_RESOLUTION;
}

void StartupData::verifyLoadedData()
{
    if(pointLB.x >= pointRT.x)
    {
        printf("Left Point should be the left one - using default values\n");

        this->pointLB.x = DEFAULT_POINT_LB_x;
        this->pointLB.y = DEFAULT_POINT_LB_Y;
        this->pointRT.x = DEFAULT_POINT_RT_X;
        this->pointRT.y = DEFAULT_POINT_RT_Y;
    }

    if(pointLB.y >= pointRT.y)
    {
        printf("Bottom Point should be the bottom one - using default values\n");

        this->pointLB.x = DEFAULT_POINT_LB_x;
        this->pointLB.y = DEFAULT_POINT_LB_Y;
        this->pointRT.x = DEFAULT_POINT_RT_X;
        this->pointRT.y = DEFAULT_POINT_RT_Y;
    }

    if(resolution <= 0.0)
    {
        printf("resolution need to be an positive number - using default");

        this->resolution = DEFAULT_RESOLUTION;
    }

    if(resolution > 1.0)
    {
        printf("resolution can't be greater than 1.0 - using 1.0");

        this->resolution = 1.0;
    }
}

void StartupData::loadData()
{
    std::ifstream file(STARTUP_DATA_FILE);

    if(!file.is_open())
    {
        fprintf(stderr, "cannot open " STARTUP_DATA_FILE " file\n");
        printf("using default data");
        return;
    }

    ignoreLine(file, false);
    file >> this->pointLB.x >> this->pointLB.y;

    ignoreLine(file);
    file >> this->pointRT.x >> this->pointRT.y;

    ignoreLine(file);
    file >> this->iterations;

    ignoreLine(file);
    file >> this->resolution;

    file.close();

    this->verifyLoadedData();

    printf("Startup data loaded correctly\n");
    printf("\t" "pointLB = {%g, %g}\n", this->pointLB.x, this->pointLB.y);
    printf("\t" "pointRT = {%g, %g}\n", this->pointRT.x, this->pointRT.y);
    printf("\t" "iterations = %d\n", this->iterations);
    printf("\t" "resolution = %g\n", this->resolution);

}

StartupData::StartupData() {
    this->setDefaultStartupData();
    this->loadData();
}

StartupData *StartupData::getInstance()
{
    static StartupData startupData;
    return &startupData;
}
