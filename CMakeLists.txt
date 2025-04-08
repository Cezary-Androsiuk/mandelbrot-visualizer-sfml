cmake_minimum_required(VERSION 3.15)
project(main)

set(CMAKE_CXX_STANDARD 17)

# Point where look for SFML stuff
set(SFML_DIR "C:/SFML-2.6.1/lib/cmake/SFML")

# Look for SFML stuff
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/cpp)

# Add resources (for visualization purposes only)
set(RESOURCE_FILES
    cpp/startupData.txt
)
source_group("Resources" FILES ${RESOURCE_FILES})

add_executable(
    ${PROJECT_NAME} 

    ${RESOURCE_FILES} # allows to copy resources within compilation

    cpp/main.cpp 
    cpp/Matrix.cpp 
    cpp/Mandelbrot.cpp
    cpp/Program.cpp
    cpp/ColorPalette.cpp
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)