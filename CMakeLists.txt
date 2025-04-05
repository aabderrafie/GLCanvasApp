cmake_minimum_required(VERSION 3.15)
project(GLCanvasApp)

set(CMAKE_CXX_STANDARD 17)

find_package(wxWidgets REQUIRED COMPONENTS core base gl adv html)
include(${wxWidgets_USE_FILE})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")

add_executable(GLCanvasApp
    main.cpp
    MainApp.cpp
    MainFrame.cpp
    GLCanvas.cpp
)

target_link_libraries(GLCanvasApp
    ${wxWidgets_LIBRARIES}
    OpenGL::GL
    GLUT::GLUT
)

if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
    
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(CARBON_LIBRARY Carbon)
    find_library(QUARTZCORE_LIBRARY QuartzCore)
    
    target_link_libraries(GLCanvasApp
        ${COCOA_LIBRARY}
        ${OPENGL_LIBRARY}
        ${IOKIT_LIBRARY}
        ${CARBON_LIBRARY}
        ${QUARTZCORE_LIBRARY}
    )
endif()