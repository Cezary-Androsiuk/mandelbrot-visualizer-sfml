# Mandelbrot Visualizer - SFML

## About
A proof-of-concept application for navigating the Mandelbrot set fractal. Developed in C++ using the SFML library.
Application uses multithreading to speed up computations and prevent locking main thread.
The implementation utilizes multithreading to accelerate computations and prevent main thread blocking. The code also includes (commented-out) support for Boost.Multiprecision, which was ultimately removed due to huge performance issues.

## Usage
ESC - exit application

⬅️ - move left
⬆️ - move up
⬇️ - move down
➡️ - move right

+ or PageUp - zoom in
- or PageDown - zoom out

## Installation
### Windows 
    The ready-to-use executable, including all necessary libraries, can be downloaded from the latest release in the "Releases" tab.

### Linux
    Must be compiled individually - no pre-built binaries provided.

### MacOS
    Must be compiled individually - no pre-built binaries provided.

## Images
![1](https://github.com/Cezary-Androsiuk/mandelbrot-visualizer-sfml/blob/master/images/1.png)

![2](https://github.com/Cezary-Androsiuk/mandelbrot-visualizer-sfml/blob/master/images/2.png)

![3](https://github.com/Cezary-Androsiuk/mandelbrot-visualizer-sfml/blob/master/images/3.png)

![4](https://github.com/Cezary-Androsiuk/mandelbrot-visualizer-sfml/blob/master/images/4.png)

![5](https://github.com/Cezary-Androsiuk/mandelbrot-visualizer-sfml/blob/master/images/5.png)

![6](https://github.com/Cezary-Androsiuk/mandelbrot-visualizer-sfml/blob/master/images/6.png)

![7](https://github.com/Cezary-Androsiuk/mandelbrot-visualizer-sfml/blob/master/images/7.png)

### Example thread usage - image generated while testing multithreading behaviour
![0](https://github.com/Cezary-Androsiuk/mandelbrot-visualizer-sfml/blob/master/images/0.png)