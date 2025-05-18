#include "Program.hpp"

#include <chrono>

#include "Support.h"

const char *version = "1.0.4";

void Program::init()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    #if DEBUG_VIEW
    desktopMode.width /= 1.4;
    desktopMode.height /= 1.4;
    #endif
    m_confData.size = {static_cast<int>(desktopMode.width), static_cast<int>(desktopMode.height)};

    m_videoMode = sf::VideoMode(desktopMode);
    m_window = new sf::RenderWindow(m_videoMode, "Mandelbrot", 
        DEBUG_VIEW ? sf::Style::Default : sf::Style::Fullscreen);
    m_window->setPosition(sf::Vector2i(10, 10));
    
    // m_window->setPosition(sf::Vector2i((MAIN_WINDOW_WIDTH - m_windowWidth)/2,(MAIN_WINDOW_HEIGHT - m_windowHeight)/2));
    m_window->setPosition(sf::Vector2i(0,0));
    m_window->setFramerateLimit(FPS);
    printf("window: %ux%u\n",m_window->getSize().x, m_window->getSize().y);
}

void Program::initLoadingShape()
{
    long double loadingShapeSize = m_videoMode.width*0.1;
    m_loadingShape.setSize(sf::Vector2f(loadingShapeSize, loadingShapeSize));
    m_loadingShape.setOrigin(sf::Vector2f(loadingShapeSize/2, loadingShapeSize/2));
    m_loadingShape.setPosition(sf::Vector2f(
        m_videoMode.width/2,
        m_videoMode.height/2
        ));
    m_loadingShape.setOutlineThickness(6.f);
    m_loadingShape.setOutlineColor(sf::Color(100,30,30));
    m_loadingShape.setFillColor(sf::Color::Transparent);
}

Program::Program()
    : m_dataWorkerThread{nullptr}
    , m_threadWorking(false)
    , m_dataNeedComputation(true)
{
    this->init();
    this->initLoadingShape();
}

Program::~Program()
{
    delete m_window;
}

void Program::makeAreaSquare(Area &area)
{
    double width = area.rightTop.x - area.leftBottom.x;
    double height = area.rightTop.y - area.leftBottom.y;

    if(width == height)
        return;

    if(width > height)
    {
        double diff = width - height;
        double halfDiff = diff/2;

        area.leftBottom.y -= halfDiff;
        area.rightTop.y += halfDiff;
    }
    else
    {
        double diff = height - width;
        double halfDiff = diff/2;

        area.leftBottom.x -= halfDiff;
        area.rightTop.x += halfDiff;
    }
}

void Program::scaleAreaToScreen(Area &area, const Size &size)
{
    double screenHeight = static_cast<double>(size.height);
    double screenWidth = static_cast<double>(size.width);

    if(screenHeight == screenWidth)
        return;

    if(screenWidth > screenHeight)
    {
        // expand area width
        double screenRatio = screenWidth / screenHeight;
        double areaWidth = area.rightTop.x - area.leftBottom.x;
        double areaWidthExpansion = areaWidth * screenRatio;
        double siteExpansion = (areaWidthExpansion - areaWidth)/2;

        area.leftBottom.x -= siteExpansion;
        area.rightTop.x += siteExpansion;
    }
    else
    {
        // expand area height
        double screenRatio = screenHeight / screenWidth;
        double areaHeight = area.rightTop.y - area.leftBottom.y;
        double areaHeightExpansion = areaHeight * screenRatio;
        double siteExpansion = (areaHeightExpansion - areaHeight)/2;

        area.leftBottom.y -= siteExpansion;
        area.rightTop.y += siteExpansion;
    }
}

void Program::displaySFML()
{
    StartupData *startupData = StartupData::getInstance();

    Program program; // shows window

    /// conpute proportions
    Program::makeAreaSquare(startupData->area);
    Program::scaleAreaToScreen(startupData->area, program.m_confData.size);

    printf("\n""transformed size\n");
    printf("%Lg %Lg\n", startupData->area.leftBottom.x, startupData->area.leftBottom.y);
    printf("%Lg %Lg\n", startupData->area.rightTop.x, startupData->area.rightTop.y);

    program.m_confData.pointLB = startupData->area.leftBottom;
    program.m_confData.pointRT = startupData->area.rightTop;
    program.m_confData.iterations = startupData->iterations;
    program.m_confData.resolution = startupData->resolution;

    program.m_confData.scalledSize = {
        (int) (program.m_confData.size.width * startupData->resolution),
        (int) (program.m_confData.size.height * startupData->resolution)
    };

    Matrix *mandelbrotMatrix = new Matrix(&program.m_confData.scalledSize);
    program.setDataMatrix(mandelbrotMatrix); // sets data that will be displayed

    while(program.m_window->isOpen())
    {
        program.pollEvent();
        program.update();
        program.render();
    }

    delete mandelbrotMatrix;
}

void Program::computeData()
{
    auto start1 = std::chrono::high_resolution_clock::now();
    constexpr int threads = COMPUTATIONS_THREADS;
    MBdata data[threads];
    std::thread* workers[threads] = {0};

    cPoint pLB = &m_confData.pointLB;
    cPoint pRT = &m_confData.pointRT;

    real dy = ((pRT->y - pLB->y)/threads);

    // printf("dy: %Lf, y: [%Lf, %Lf]\n", dy, pLB->y, pRT->y);
    // fflush(stdout);

    const real heightPerThread = m_confData.scalledSize.height / threads;

    for(int i=0; i<threads; i++)
    {
        real tmpBottom = pLB->y + dy * i;
        real bottom = tmpBottom > pRT->y ? pRT->y : tmpBottom;
        real top = pLB->y + dy * (i+1);


        // printf("y: [%Lg, %Lg]\n", bottom, top);
        // fflush(stdout);

        computeMandelbrotPart(i, pLB->x, bottom, pRT->x, top,
                          m_confData.scalledSize.width, heightPerThread,
                          heightPerThread * i, m_confData.iterations, m_dataMatrix);

        // workers[i] = new std::thread(
        //     &computeMandelbrotPart, i, pLB->x, bottom, pRT->x, top,
        //     m_confData.scalledSize.width, heightPerThread,
        //     m_confData.iterations, m_dataMatrix);
    }

    // for(int i; i<threads; i++)
    // {
    //     workers[i]->join();
    //     delete workers[i];
    // }

    // computeMandelbrot(
    //     m_confData.pointLB.x,
    //     m_confData.pointLB.y,
    //     m_confData.pointRT.x,
    //     m_confData.pointRT.y,
    //     m_confData.scalledSize.width,
    //     m_confData.scalledSize.height,
    //     m_confData.iterations,
    //     m_dataMatrix
    //     );

    auto end1 = std::chrono::high_resolution_clock::now();

    // find middle point and print it

    // printf("computation finished\n");
    auto start2 = std::chrono::high_resolution_clock::now();

    /// ~10ms
    int width = m_confData.scalledSize.width;
    int height = m_confData.scalledSize.height;
    sf::Image image;
    image.create(width, height);

    /// ~110ms
    for(int y=0; y<height; ++y)
    {
        for(int x=0; x<width; ++x)
        {
            const sf::Color &pixelColor = m_dataMatrix->getColorCell(x, y);
            image.setPixel(x, y, pixelColor);

            #if MORE_DEBUG
            if(x==y)
                printf("(%d %d %d) ", pixelColor.r, pixelColor.g, pixelColor.b);
            #endif
        }
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    #if DEBUG_MODE
    printf("created image\n");
    #endif


    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2);

    printf("%lld\n", duration1.count());
    printf("%lld\n", duration2.count());

    m_backgroundImageTexture.loadFromImage(image); //! mutex required !!!!
    // race condition... (this and drawing sprite)

    m_threadWorking = false;
    m_dataNeedComputation = false;
}

void Program::setDataMatrix(Matrix *dataMatrix)
{
    m_dataMatrix = dataMatrix;
}

void Program::startComputation()
{
    bool dataNeedComputation = m_dataNeedComputation.load();

    if(m_threadWorking) return;
    if(!dataNeedComputation) return;
    if(m_dataWorkerThread) return;

    #if DEBUG_MODE
    printf("about to start thread\n");
    #endif

    m_threadWorking = true;
    m_dataWorkerThread = new std::thread(&Program::computeData, this);
}

void Program::endComputation()
{
    if(m_threadWorking) return;
    if(!m_dataWorkerThread) return;

    m_dataWorkerThread->join(); // safe purpose

    delete m_dataWorkerThread;
    m_dataWorkerThread = nullptr;
}

void Program::zoomInView()
{
    #if DEBUG_MODE
    printf("zoom in\n");
    #endif
    
    long double widthChartSize = m_confData.pointRT.x - m_confData.pointLB.x;
    long double heightChartSize = m_confData.pointRT.y - m_confData.pointLB.y;

    long double removeWidthPerSite = (widthChartSize * ZOOM_SPEED)/2;
    long double removeHeightPerSite = (heightChartSize * ZOOM_SPEED)/2;

    m_confData.pointLB.x += removeWidthPerSite;
    m_confData.pointLB.y += removeHeightPerSite;
    m_confData.pointRT.x -= removeWidthPerSite;
    m_confData.pointRT.y -= removeHeightPerSite;

    m_dataNeedComputation = true;
}

void Program::zoomOutView()
{
    #if DEBUG_MODE
    printf("zoom out\n");
    #endif

    long double widthChartSize = m_confData.pointRT.x - m_confData.pointLB.x;
    long double heightChartSize = m_confData.pointRT.y - m_confData.pointLB.y;

    long double removeWidthPerSite = (widthChartSize * ZOOM_SPEED)/2;
    long double removeHeightPerSite = (heightChartSize * ZOOM_SPEED)/2;

    m_confData.pointLB.x -= removeWidthPerSite;
    m_confData.pointLB.y -= removeHeightPerSite;
    m_confData.pointRT.x += removeWidthPerSite;
    m_confData.pointRT.y += removeHeightPerSite;

    m_dataNeedComputation = true;
}

void Program::moveViewUp()
{
    #if DEBUG_MODE
    printf("move top\n");
    #endif
    
    long double heightChartSize = m_confData.pointRT.y - m_confData.pointLB.y;
    long double removeHeightPerSite = (heightChartSize * ZOOM_SPEED)/2;

    m_confData.pointLB.y -= removeHeightPerSite;
    m_confData.pointRT.y -= removeHeightPerSite;

    m_dataNeedComputation = true;
}

void Program::moveViewLeft()
{
    #if DEBUG_MODE
    printf("move left\n");
    #endif

    long double widthChartSize = m_confData.pointRT.x - m_confData.pointLB.x;
    long double removeWidthPerSite = (widthChartSize * ZOOM_SPEED)/2;

    m_confData.pointLB.x -= removeWidthPerSite;
    m_confData.pointRT.x -= removeWidthPerSite;

    m_dataNeedComputation = true;
}

void Program::moveViewRight()
{
    #if DEBUG_MODE
    printf("move right\n");
    #endif

    long double widthChartSize = m_confData.pointRT.x - m_confData.pointLB.x;
    long double removeWidthPerSite = (widthChartSize * ZOOM_SPEED)/2;

    m_confData.pointLB.x += removeWidthPerSite;
    m_confData.pointRT.x += removeWidthPerSite;

    m_dataNeedComputation = true;
}

void Program::moveViewDown()
{
    #if DEBUG_MODE
    printf("move bottom\n");
    #endif

    long double heightChartSize = m_confData.pointRT.y - m_confData.pointLB.y;
    long double removeHeightPerSite = (heightChartSize * ZOOM_SPEED)/2;
    
    m_confData.pointLB.y += removeHeightPerSite;
    m_confData.pointRT.y += removeHeightPerSite;

    m_dataNeedComputation = true;
}

void Program::checkMoves()
{
    if(m_currentEvent.key.code == sf::Keyboard::Escape)
        m_window->close();

    /// prevents queuing actions
    if(m_threadWorking)
        return;
    
    if(m_currentEvent.key.code == sf::Keyboard::Add)
        this->zoomInView();
    if(m_currentEvent.key.code == sf::Keyboard::PageUp)
        this->zoomInView();
    if(m_currentEvent.key.code == sf::Keyboard::Subtract)
        this->zoomOutView();
    if(m_currentEvent.key.code == sf::Keyboard::PageDown)
        this->zoomOutView();

    if(m_currentEvent.key.code == sf::Keyboard::Up)
        this->moveViewUp();
    if(m_currentEvent.key.code == sf::Keyboard::Left)
        this->moveViewLeft();
    if(m_currentEvent.key.code == sf::Keyboard::Right)
        this->moveViewRight();
    if(m_currentEvent.key.code == sf::Keyboard::Down)
        this->moveViewDown();

    if(m_dataNeedComputation)
        this->printNewViewBounds();
}

void Program::printNewViewBounds()
{
    printf("New view Bounds: LB={%.20Lf, %.20Lf}, Rt{%.20Lf, %.20Lf}\n",
           m_confData.pointLB.x, m_confData.pointLB.y,
           m_confData.pointRT.x, m_confData.pointRT.y);
    fflush(stdout);
}

void Program::pollEvent()
{
    while(m_window->pollEvent(m_currentEvent)){
        switch (m_currentEvent.type){
        case sf::Event::Closed:
            m_window->close();
            break;
        case sf::Event::KeyPressed:
            this->checkMoves();
            break;
        default:
            break;
        }
    }
}

void Program::update()
{
    this->startComputation();
    this->endComputation();
    

    #if MORE_DEBUG
    static int i=0;
    printf("update %d\n", i++);
    if(i == 20)
        m_dataNeedComputation = true;
    #endif

    if(m_threadWorking)
    {
        m_loadingShape.rotate(LOADING_ROTATION_SPEED);
    }
    else
    {
        m_loadingShape.setRotation(0);
    }
}

void Program::render()
{
    m_window->clear(BACKGROUND_SF_COLOR);

    sf::Sprite sprite(m_backgroundImageTexture);
    long double spriteScale = 1/m_confData.resolution;
    sprite.setScale(sf::Vector2f(spriteScale, spriteScale));
    m_window->draw(sprite);
    /// idk why sprite need to be created here, 
    /// if i create it as an class member then nothing shows up

    if(m_threadWorking)
    {
        m_window->draw(m_loadingShape);
    }

    m_window->display();
}
