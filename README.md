# Lotka-Volterra prey-predator simulation

---

## Description
C++ project for academic use: Lotka-Volterra prey-predator simulation, unit tests included (Doctest), and graphical rendering using SFML.

---

## Project Structure
- **include/**: header files
    - _input.hpp_
    - _io.hpp_
    - _lotka_volterra.hpp_
    - _output.hpp_
    - _renderer.hpp_
    - _simulation.hpp_
- **io/**: input/output files
    - _input.cpp_
    - _output.cpp_
- **src/**: source files
    - _main.cpp_
    - _renderer.cpp_
    - _simulation.cpp_
- **test/**: unit tests
    - _main_test.cpp_
    - _renderer_test.cpp_
    - _simulation_test.cpp_
- _CMakeLists.txt_: build configuration
- _doctest.h_: testing framework
- _font.ttf_: font file
- _README.md_: this file

---

## Instructions
- **Installation of ninja (build system)**
    in _linux_:
    ```bash
    $ sudo apt install ninja-build
    ```
    in _macOs_:
    ```bash
    $ brew install ninja
    ```
    to verify:
    ```bash
    ninja --version
    ```
- 