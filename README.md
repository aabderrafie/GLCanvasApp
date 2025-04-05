# GLCanvasApp - Interactive OpenGL Canvas with wxWidgets Controls

A modern C++ application that demonstrates the integration of OpenGL rendering with wxWidgets UI framework. This project features a 3D scene with interactive controls, OpenGL-rendered UI elements, and responsive design.

## Features

- 3D rendering of various models (cube, sphere, torus, teapot)
- Modern UI overlay rendered directly in OpenGL
- Interactive controls for manipulating the scene:
  - Rotation control
  - Wireframe toggle
  - Object and background color selection
  - Model selection
- Responsive design that handles window resizing
- Proper OpenGL context management

## Requirements

- C++17 compatible compiler (Clang recommended)
- wxWidgets 3.x
- OpenGL
- GLUT
- CMake 3.15 or higher

## Building the Project

### Build Instructions

1. Clone the repository:
   ```
   git clone https://github.com/aabderrafie/GLCanvasApp.git
   cd GLCanvasApp
   ```

2. Create a build directory:
   ```
   mkdir build
   cd build
   ```

3. Run CMake:
   ```
   cmake ..
   ```

4. Build the project:
   ```
   make
   ```

5. Run the application:
   ```
   ./GLCanvasApp
   ```

## Usage

- The application starts with a 3D scene showing a cube.
- The overlay at the top contains two buttons:
  - "Controls" - Toggles the control panel visibility
  - "Settings" - For future settings functionality
- The control panel allows you to:
  - Adjust the rotation angle with a slider or by entering a precise value
  - Toggle wireframe mode
  - Change background and object colors
  - Select different 3D models (cube, sphere, torus, teapot)

## Project Structure

- `main.cpp` - Application entry point
- `MainApp.cpp/h` - wxWidgets application class
- `MainFrame.cpp/h` - Main application window
- `GLCanvas.cpp/h` - OpenGL rendering canvas and event handling

## Troubleshooting

- **OpenGL Context Issues**: If you encounter problems with the OpenGL context, make sure your graphics drivers are up to date.
- **Build Errors**: Ensure you have all required libraries installed and properly linked.
- **wxWidgets Version**: This project was tested with wxWidgets 3.2. Earlier or later versions may require adjustments.
