#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <atomic>
#include <thread>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "Matrix.hpp"
#include "Mandelbrot.hpp"
#include "ColorPalette.hpp"

#define DEBUG_VIEW false // makes window smaller
#define DEBUG_MODE false // add some comments
#define MORE_DEBUG false // add even more comments

#define FPS 30

#define BACKGROUND_SF_COLOR sf::Color(30, 30, 30)

/// describes how by what percentage of the current size (width/height) 
/// the space is to be moved
#define ZOOM_SPEED 0.2
#define MOVE_SPEED 0.2 

#define LOADING_ROTATION_SPEED 10.f

class Program{

    void init();

    Program();
    ~Program();

public:
    static void displaySFML(cPoint pointLB, cPoint pointRT, int iterations, long double resolution);

private:
    void computeData();

private:
    void setDataMatrix(Matrix *dataMatrix);

    void startComputation();
    void endComputation();

    void zoomInView();
    void zoomOutView();
    void moveViewUp();
    void moveViewLeft();
    void moveViewRight();
    void moveViewDown();
    void checkMoves();

    void pollEvent();
    void update();
    void render();

private:
    struct{
        Point pointLB; 
        Point pointRT; 
        Size size; 
        Size scalledSize;
        int iterations;
        long double resolution;
    } m_confData;
    Matrix *m_dataMatrix;
    std::thread *m_dataWorkerThread;
    // std::atomic<bool> m_dataComputed;
    bool m_threadWorking;
    std::atomic<bool> m_dataNeedComputation;

    
    sf::RenderWindow* m_window;
    sf::VideoMode m_videoMode;
    sf::Event m_currentEvent;

    sf::Texture m_backgroundImageTexture;

    sf::RectangleShape m_loadingShape;
};


#endif // PROGRAM_HPP