# Lotka-Volterra prey-predator simulation

---

## Description
C++ project for academic use: Lotka-Volterra prey-predator simulation, unit tests included (Doctest), and graphical rendering using SFML.

---

## Project Structure
- **include/**: headesrmaibdo for files
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

- **Installation of SFML (graphics library)**

    Install SFML:

    in _linux_
    ```bash
    sudo apt install libsfml-dev
    Reading package lists... Done
    Building dependency tree... Done
    Reading state information... Done
    ...
    ```

    in _macOs_
    ```bash
    brew install sfml
    ==> Downloading https://ghcr.io/v2/homebrew/core/sfml/manifests/2.5.1
    ==> Downloading from https://ghcr.io/v2/homebrew/core/sfml/blobs/sha256:<hash>
    ...
    ```

- **Installation of ninja (build system)**

    Install the ninja-build-system:

    in _linux_
    ```bash
    $ sudo apt install ninja-build
    Reading package lists... Done
    Building dependency tree... Done
    Reading state information... Done
    ...
    ```

    in _macOs_
    ```bash
    $ brew install ninja
    ==> Downloading https://ghcr.io/v2/homebrew/core/ninja/manifests/1.11.1
    ==> Downloading from https://ghcr.io/v2/homebrew/core/ninja/blobs/sha256:<hash>
    ...
    ```

    To verify the installation:
    ```bash
    ninja --version
    1.11.1
    ```

- **Creation of work area**

    Create directory `/home/user/project_name` (with structure above):
    ```bash
    $ cd ~
    $ mkdir project_name
    ```
    
    Alternatively, unzip `/home/user/project_name_.zip` (`/home/user/project-main.zip` if downloaded directly from github):
    ```bash
    $ cd ~
    $ unzip project_name.zip
    Archive:  project_name_.zip
    ...
    ```

    Now, go inside the work area:
    ```bash
    $ cd project_name
    ```

- **Creation of compile area**

    Create and configure directory `project_name/build`, using `CMakeLists.txt` file in current directory (`.`):
    ```bash
    $ cmake -S . -B build -G"Ninja Multi-Config"
    -- The C compiler identification is GNU 13.3.0
    -- The CXX compiler identification is GNU 13.3.0
    -- Detecting C compiler ABI info
    -- Detecting C compiler ABI info - done
    -- Check for working C compiler: /usr/bin/cc - skipped
    -- Detecting C compile features
    -- Detecting C compile features - done
    -- Detecting CXX compiler ABI info
    -- Detecting CXX compiler ABI info - done
    -- Check for working CXX compiler: /usr/bin/c++ - skipped
    -- Detecting CXX compile features
    -- Detecting CXX compile features - done
    -- Configuring done (1.0s)
    -- Generating done (0.1s)
    -- Build files have been written to: /home/project_name/build
    ```

- **Compiling and testing**

    First, compile the program in _Debug_ mode and test it:
    ```bash
    $ cmake --build build --config Debug
    [11/11] Linking CXX executable Debug/project_test
    $ cmake --build build --config Debug --target test
    [0/1] Running tests...
    Test project /home/tommaso/project/build
        Start 1: project_test
    1/1 Test #1: project_test .....................   Passed    0.60 sec

    100% tests passed, 0 tests failed out of 1

    Total Test time (real) =   0.61 sec
    ```

    Then, compile it in _Release_ mode and test it:
    ```bash
    $ cmake --build build --config Release
    [11/11] Linking CXX executable Release/project
    $ cmake --build build --config Release --target test
    [0/1] Running tests...
    Test project /home/tommaso/project/build
        Start 1: project_test
    1/1 Test #1: project_test .....................   Passed    0.27 sec

    100% tests passed, 0 tests failed out of 1

    Total Test time (real) =   0.28 sec
    ```

- **Running**

    First, run the executable compiled in _Debug_ mode:
    ```bash
    $ ./build/Debug/project
    ...
    ```

    Insert the needed parameters and enjoy the rendering.

    Then run that compiled in _Release_ mode:
    ```bash
    $ ./build/Release/project
    ...
    ```

    Again, insert the needed parameters and try yourself!