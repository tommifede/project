<!-- omit in toc -->
# Lotka–Volterra Prey–Predator Simulation

Project developed by Tommaso Federici.

---

- [Lotka–Volterra Prey–Predator Simulation](#lotkavolterra-preypredator-simulation)
    - [Introduction](#introduction)
    - [Descriptions](#description)
    - [Implementation](#implementation)
        - [Project Structure](#project-structure)
        - [Design choices](#design-choices)
    - [Compilation](#compilation)
        - [Requirements](#requirements)
    - [Author](#author)

---

## Introduction
This is a C++ project developed for academic purposes.  
It implements a Lotka–Volterra prey–predator simulation, aiming to collect initial conditions and parameters and to simulate the system's evolution using a discrete-time model.  
It includes unit tests (with Doctest) and graphical rendering (with the SFML library).  

---

## Description
Lotka–Volterra equations are used to describe a simplified prey–predator
interaction in a given ecosystem:

$$
\begin{align*}
    \frac{dx}{dt} &= (A - B y(t)) x(t)\\
    \frac{dy}{dt} &= (C x(t) - D ) y(t)\\
\end{align*}
$$

$x(t)$ and $y(t)$ denote the prey and predator populations at time $t$, with $A$ and $C$ representing
their intrinsic reproduction coefficients under sufficient food
availability, and $B$ and $D$ representing their
respective mortality coefficients.
The solution of the system of differential equations has two equilibrium points:

$$
\begin{align*}
    e_{1} &= (0, 0)\\
    e_{2} &= \left(\frac{D}{C}, \frac{A}{B} \right)\\
\end{align*}
$$

By discretizing the Lotka–Volterra equations, we obtain:

$$
\begin{align*}
x_i &= x_{i-1} + (A - B  y_{i-1}) x_{i-1} \Delta t\\
y_i &= y_{i-1} + (C x_{i-1} - D ) y_{i-1} \Delta t\\
\end{align*}
$$

Furthermore, by expressing the variables $(x_i, y_i)$ as fractions of the second
equilibrium point:

$$
\begin{align*}
x_i^{rel} &= x_{i} \frac{C}{D} \\
y_i^{rel} &= y_{i} \frac{B}{A} \\
\end{align*}
$$

the discretized equations become:

$$\begin{align*}
x_i^{rel} &= x_{i-1}^{rel} + A (1 - y_{i-1}^{rel}) x_{i-1}^{rel} \Delta t\\
y_i^{rel} &= y_{i-1}^{rel} + D (x_{i-1}^{rel} - 1) y_{i-1}^{rel} \Delta t\\
\end{align*}
$$

The system possesses a first integral that is conserved over time:

$$
\begin{align*}
H(x,y) &= -D\ln(x)+Cx+By-A\ln(y)
\end{align*}
$$

Henceforth, the first integral will be denoted as the energy of the system.

---

## Implementation

### Project Structure
The project is organized in a hierarchical, tree-like directory structure.  
The root directory contains configuration and utility files, while the source code is divided into 4 dedicated subdirectories as described below:

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

### Design Choices
a

---

## Compilation
> [!NOTE]
> Make sure you have a C++20–compliant compiler
> If your compiler does not support C++20, please update it or use an alternative compiler that does.

- **Installation of SFML (graphics library)**

    Make sure you have **SFML** (version 2.6 or later) installed (used for graphical output).
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

    Make sure you have **CMake** (version 3.28 or later) and **Ninja** installed.
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

## Author
**Tommaso Federici**  
Alma Mater Studiorum – University of Bologna  
Bachelor's Degree in Physics (Second Year)  
Email: tommaso.federici3@studio.unibo.it
