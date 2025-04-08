#include "Program.hpp"


void Program::init()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    #if DEBUG_VIEW
    desktopMode.width /= 1.2;
    desktopMode.height /= 1.2;
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
}

Program::~Program()
{
    delete m_window;
}

void Program::displaySFML(StartupData *startupData)
{
    Program program; // shows window
    program.m_confData.pointLB = startupData->pointLB;
    program.m_confData.pointRT = startupData->pointRT;
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
    computeMandelbrot(
        & m_confData.pointLB,
        & m_confData.pointRT,
        & m_confData.scalledSize,
        m_confData.iterations,
        m_dataMatrix
    );

    printf("computation finished\n");

    const long double scale = 256.0 / m_confData.iterations;

    int width = m_confData.scalledSize.width;
    int height = m_confData.scalledSize.height;
    sf::Image image;
    image.create(width, height);

    for(int y=0; y<height; ++y)
    {
        for(int x=0; x<width; ++x)
        {
            int cell = m_dataMatrix->getCell(x, y);
            int colorIndex = (int) floor(5.0*scale*log2f(1.0f*cell+1));

            sf::Color pixelColor;
            pixelColor.r = ColorPalette[colorIndex][0];
            pixelColor.g = ColorPalette[colorIndex][2];
            pixelColor.b = ColorPalette[colorIndex][1];

            image.setPixel(x, y, pixelColor);

            #if MORE_DEBUG
            if(x==y)
                printf("(%d %d %d) ", pixelColor.r, pixelColor.g, pixelColor.b);
            #endif 
        }
    }

    #if DEBUG_MODE
    printf("created image\n");
    #endif

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
    m_dataWorkerThread = new std::thread(Program::computeData, this);
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
}

void Program::pollEvent()
{
    while(m_window->pollEvent(m_currentEvent)){
        switch (m_currentEvent.type){
        case sf::Event::Closed:
            m_window->close();
            break;
        case sf::Event::KeyPressed:
            if(m_currentEvent.key.code == sf::Keyboard::Escape)
                m_window->close();

            this->checkMoves();
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
