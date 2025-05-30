cmake_minimum_required(VERSION 3.16)

project(MandelbrotVisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Point where look for Boost stuff
# set(BOOST_ROOT "C:/boost_1_88_0")

# Point where look for SFML stuff
set(SFML_DIR "C:/SFML-2.6.1/lib/cmake/SFML")

# Look for Boost stuff
# find_package(Boost 1.88.0 REQUIRED)

# Look for SFML stuff
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# set Boost include directory # not required if Boost is added to env variables
# include_directories(${Boost_INCLUDE_DIRS})

# set SFML include directory # not required if Boost is added to env variables


include_directories(${CMAKE_SOURCE_DIR}/cpp)

# Add resources (for visualization purposes only)
set(RESOURCE_FILES
    resources/startupData.txt
)
source_group("Resources" FILES ${RESOURCE_FILES})

add_executable(
    ${PROJECT_NAME}

    ${RESOURCE_FILES} # allows to copy resources within compilation

    cpp/main.cpp
    cpp/Matrix.cpp cpp/Matrix.hpp
    cpp/Mandelbrot.cpp cpp/Mandelbrot.hpp
    cpp/Program.cpp cpp/Program.hpp
    cpp/ColorPalette.cpp cpp/ColorPalette.hpp
    cpp/StartupData.h cpp/StartupData.cpp
    cpp/Point.h cpp/Point.cpp
    cpp/Size.h cpp/Size.cpp
    cpp/Area.h cpp/Area.cpp
    cpp/Support.h cpp/Support.cpp
    cpp/Real.h cpp/Real.cpp
)

file(GLOB RESOURCE_FILES_LIST "${CMAKE_SOURCE_DIR}/resources/*")

add_custom_target(resources_copy ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        ${CMAKE_BINARY_DIR}/resources
    DEPENDS ${RESOURCE_FILES_LIST}
    COMMENT "Copying resources to build directory"
)

add_dependencies(${PROJECT_NAME} resources_copy)


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
