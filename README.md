# Lotka–Volterra Prey–Predator Simulation

Project developed by Tommaso Federici.

---

- [Lotka–Volterra Prey–Predator Simulation](#lotkavolterra-preypredator-simulation)
    - [Description](#description)
    - [Features](#features)
    - [Requirements](#requirements)
    - [Project Structure](#project-structure)
    - [Instructions](#instructions)
    - [Implementation](#implementation)
    - [Author](#author)

---

## Description
This is a C++ project developed for academic purposes.
It implements a Lotka–Volterra prey–predator simulation, aiming to collect initial conditions and parameters and to simulate the system's evolution using a discrete-time model.
It includes unit tests (with Doctest) and graphical rendering (with the SFML library).

---

## Features
- Discrete-time Lotka–Volterra model
- Interactive input of simulation parameters
- Real-time graphical visualization using SFML
- Unit tests implemented with Doctest

---

## Requirements
- CMake 3.28 or later
- C++20–compliant compiler
- SFML 2.6 or later (graphics module)
- Ninja build system (required for the provided build instructions)

---

## Project Structure
- **include/**: header files
    - _input.hpp_
    - _output.hpp_
    - _renderer.hpp_
    - _simulation.hpp_
- **io/**: input/output implementation files
    - _input.cpp_
    - _output.cpp_
- **src/**: source files
    - _main.cpp_
    - _renderer.cpp_
    - _simulation.cpp_
- **test/**: unit tests
    - _renderer_test.cpp_
    - _simulation_test.cpp_
- _CMakeLists.txt_: build configuration
- _doctest.h_: testing framework
- _font.ttf_: font file
- _README.md_: this file

---

## Instructions

- **Installation of SFML (graphics library)**

    Make sure you have **SFML** installed (used for graphical output).
    If not, please install it through the commands below.

    On _Linux_
    ```bash
    $ sudo apt install libsfml-dev
    ...
    ```

    On _macOS_
    ```bash
    % brew install sfml
    ...
    ```

- **Installation of CMake and Ninja (build system)**

    Make sure you have **CMake** and **Ninja** installed.
    If not, please install them through the commands below.

    On _Linux_
    ```bash
    $ sudo apt install cmake
    ...
    $ sudo apt install ninja-build
    ...
    ```

    On _macOS_
    ```bash
    % brew install cmake
    ...
    % brew install ninja
    ...
    ```

    To verify the installation:
    ```bash
    $ cmake --version
    ...
    $ ninja --version
    ...
    ```

- **Creation of work area**

    Create the directory `/home/user/project_name` (with the structure described above):
    ```bash
    $ cd ~
    $ mkdir project_name
    ```
    
    Alternatively, unzip `/home/user/project_name.zip` (`/home/user/project-main.zip` if downloaded directly from GitHub):
    ```bash
    $ cd ~
    $ unzip project_name.zip
    ...
    ```

    Now, go inside the work area:
    ```bash
    $ cd project_name
    ```

- **Creation of compile area**

    Create and configure directory `project_name/build`, using the `CMakeLists.txt` file in the current directory (`.`):
    ```bash
    $ cmake -S . -B build -G"Ninja Multi-Config"
    ...
    ```

- **Compiling and testing**

    First, compile the program in _Debug_ mode and test it:
    ```bash
    $ cmake --build build --config Debug
    $ cmake --build build --config Debug --target test
    ```

    Then, repeat the same steps in _Release_ mode:
    ```bash
    $ cmake --build build --config Release
    $ cmake --build build --config Release --target test
    ```

- **Running**

    First, run the executable compiled in _Debug_ mode:
    ```bash
    $ ./build/Debug/project
    ...
    ```

    Insert the required parameters to start the simulation and view the graphical visualization.

    Then run that compiled in _Release_ mode:
    ```bash
    $ ./build/Release/project
    ...
    ```

    Again, insert the required parameters and try yourself!

---

## Implementation
The core simulation logic is implemented in `simulation.cpp`, following the classical Lotka–Volterra equations discretized in time.

---

## Author
**Tommaso Federici**  
Alma Mater Studiorum – University of Bologna  
Bachelor's Degree in Physics (Second Year)  
Email: tommaso.federici3@studio.unibo.it
