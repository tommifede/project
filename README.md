# Lotka-Volterra prey-predator simulation

## Description
C++ project for academic use: Lotka-Volterra prey-predator simulation, unit tests included (Doctest), and graphical rendering using SFML.

## Project Structure
- "src/" : source files (".cpp")
- "include/" : header files (".hpp")
- "test/" : unit tests
- "CMakeLists.txt" : build configuration
- "doctest.h" : testing framework
- "README.md" : this file

project/
├─ include/                # Header files (.hpp)
│   ├─ namespace.hpp       # Namespace definition
│   ├─ simulation.hpp      # Simulation class
│   └─ renderer.hpp        # Renderer class
├─ src/                    # Source files (.cpp)
│   ├─ simulation.cpp
│   ├─ renderer.cpp
│   └─ main.cpp
├─ tests/                  # Unit tests
│   └─ simulation_tests.cpp
├─ resources/              # Optional images, fonts
├─ CMakeLists.txt          # Build configuration
├─ doctest.h               # Testing framework
└─ README.md               # This file

