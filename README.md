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

    Install the ninja-build-system:
    
    In _linux_
    ```bash
    $ sudo apt install ninja-build
    Reading package lists... Done
    Building dependency tree... Done
    Reading state information... Done
    The following NEW packages will be installed:
    ninja-build
    ...
    ```

    in _macOs_
    ```bash
    $ brew install ninja
    ==> Downloading https://formulae.brew.sh/api/formula.jws.json
    ==> Downloading https://formulae.brew.sh/api/cask.jws.json
    ==> Downloading https://ghcr.io/v2/homebrew/core/ninja/manifests/1.12.1-1
    Already downloaded: /Users/battilan/Library/Caches/Homebrew/downloads/e245d32f82082268a4093a10343e3ef9e2b2eebc81897551ebe10cdea0f45051--ninja-1.12.1-1.bottle_manifest.json
    ==> Fetching ninja
    ...
    ```

    To verify the installation:
    ```bash
    ninja --version
    1.13.1
    ```

- **Work area**

    Create directory `project` (with structure above):
    ```bash
    $ mkdir project
    ```
    
    or unzip `project.zip`/`project.tar.gz`:
    ```bash
    $ tar -xvf project.tar.gz
    ```
    or
    ```bash
    $ unzip project.zip
    ```