# Gemini Project: SnakeGame

## Project Overview

This is a classic Snake game implemented in C++.

- **Language:** C++17
- **Graphics:** OpenGL
- **Windowing & Input:** GLFW
- **Math:** GLM (OpenGL Mathematics)
- **Build System:** CMake

The project is structured with header files in `include/`, source files in `src/`, and shaders in `res/shaders/`. The main application logic is in `src/main.cpp`. The game features a snake that can be controlled by the player, food that appears in random locations, collision detection, and a scoring system.

## Prerequisites

- **CMake**: This project uses CMake for building. Make sure you have CMake installed. You can find installation instructions at [https://cmake.org/install/](https://cmake.org/install/).

## Building and Running

This project uses CMake to build.

**1. Configure the project:**

```bash
cmake -B build
```

**2. Build the project:**

```bash
cmake --build build
```

**3. Run the game:**

```bash
./build/SnakeGame
```

*NOTE: These build commands have not been verified as `cmake` was not found in the environment.*

## Development Conventions

- The code is organized into classes, with `Snake` and `Shader` being the main ones.
- Header files are located in the `include` directory.
- Source files are located in the `src` directory.
- Shaders have their own directory under `res/shaders`.
- The code uses modern C++ features (C++17).
- The project uses external libraries like `glfw` and `glm`, which are fetched during the CMake configuration process.