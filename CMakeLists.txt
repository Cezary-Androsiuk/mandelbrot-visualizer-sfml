cmake_minimum_required(VERSION 3.15)
project(main)

# Wymagany standard C++
set(CMAKE_CXX_STANDARD 17)

# Wskaż CMake gdzie szukać SFML (ważne dla Windows!)
set(SFML_DIR "C:/SFML-2.6.1/lib/cmake/SFML")  # Ścieżka do CMake SFML

# Szukaj biblioteki SFML
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Utwórz plik wykonywalny
add_executable(
    ${PROJECT_NAME} 
    cpp/main.cpp 
    cpp/Matrix.cpp 
    cpp/Mandelbrot.cpp
    cpp/Program.cpp
    cpp/ColorPalette.cpp
)

# Podlinkuj biblioteki SFML
target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)