# Lotka–Volterra Prey–Predator Simulation

Project developed by Tommaso Federici.

---

- [Lotka–Volterra Prey–Predator Simulation](#lotkavolterra-preypredator-simulation)
    - [Introduction](#introduction)
    - [Descriptions](#description)
    - [Implementation](#implementation)
        - [Project Structure](#project-structure)
        - [Design choices](#design-choices)
            - [Simulation](#simulation)
            - [Renderer](#renderer)
            - [I/O](#io)
            - [Main](#main)
    - [Compilation](#compilation)
    - [Results](#results)
    - [Testing](#testing)
    - [Use of AI](#use-of-ai)
    - [Author](#author)

---

## Introduction
This is a C++ project developed for academic purposes.  
It implements a **Lotka–Volterra prey–predator simulation**, aiming to collect initial conditions and parameters and to simulate the system's evolution using a **discrete-time model**.  
It includes unit tests (with **Doctest**) and graphical rendering (with the **SFML** library).  

---

## Description
**Lotka–Volterra equations** are used to describe a simplified prey–predator
interaction in a given ecosystem:

$$
\begin{align*}
    \frac{dx}{dt} &= (A - B y(t)) x(t)\\
    \frac{dy}{dt} &= (C x(t) - D ) y(t)\\
\end{align*}
$$

$x(t)$ and $y(t)$ denote the prey and predator populations at time $t$, with $A$ and $C$ representing
their **intrinsic reproduction coefficients** under sufficient food
availability, and $B$ and $D$ representing their
respective **mortality coefficients**.    
    
The solution of the system of differential equations has two **equilibrium points**:

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

the **discretized equations** become:

$$\begin{align*}
x_i^{rel} &= x_{i-1}^{rel} + A (1 - y_{i-1}^{rel}) x_{i-1}^{rel} \Delta t\\
y_i^{rel} &= y_{i-1}^{rel} + D (x_{i-1}^{rel} - 1) y_{i-1}^{rel} \Delta t\\
\end{align*}
$$

The system possesses a **first integral** that is conserved over time:

$$
\begin{align*}
H(x,y) &= -D\ln(x)+Cx+By-A\ln(y)
\end{align*}
$$

Henceforth, the first integral will be denoted as the **energy** of the system.

---

## Implementation
Hereinafter, the main project decisions are described and motivated.

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
The project is structured in a modular way, separating numerical simulation, graphical rendering, and input/output handling into independent components.    
All the code related to the Lotka–Volterra model is contained in the `lotka_volterra` namespace, while input/output functionality is implemented within the `io` namespace.

#### Simulation
The simulation of the Lotka–Volterra system is implemented through a `Simulation` class.    
The state of the system at each time step is represented by a `State` struct, containing the populations $x$, $y$ and the value of the first integral $H$.    
The `Simulation` class stores the time step `dt_`, the relative variables `x_rel_` and `y_rel_`, a vector of states `states_`, and a vector of model parameters `pars_`.    
    
Private methods are used to check parameter validity, perform a single integration step, and compute the energy. The public interface allows access to the simulation data and provides methods to evolve the system by one step, by a fixed number of steps, or over a given time interval.

All simulation parameters are verified at **run time** whenever they are provided to the system – either through the constructor, the `getParameter` accessor, or the `evolveTime` method – to ensure they are physically meaningful and numerically valid (e.g. positive populations, strictly positive time step, valid coefficients).    
    
Checks are performed using `throw` statements rather than `assert`, allowing errors to be reported clearly and preventing the simulation from evolving with inconsistent or invalid configurations.

#### Renderer
The `Renderer` class handles the graphical representation of the Lotka–Volterra simulation using SFML.    
It separates private members, which store internal state such as window size, axis scales, maximum world extents, trajectory points, last drawn step, text labels and others, from public methods that provide the drawing interface and configuration.    
    
Private methods compute tick steps, world scaling, and manage incremental trajectory updates via `last_drawn_step_` and `sf::VertexArray`, ensuring that only newly evolved points are processed at each step to optimize performance. The public interface includes the constructor, a getter for window size, a `setDraw` method to configure views and scaling based on current simulation state, and two overloaded `draw` methods: one to render up to a specific step, useful for animated evolution, and one to draw the full trajectory at once.
    
Rendering uses two separate views: `ui_view` for fixed interface elements such as axes, labels, and titles, and `world_view` for the dynamic simulation space, allowing independent scaling and panning without affecting the user interface.    
Trajectory points are colored according to deviations from the initial energy, providing a direct visual cue of the system's energetic changes, while equilibrium points are highlighted in distinct colors.    
    
Tick steps and axis scales are dynamically computed from the simulation's current maximum populations and a configurable margin, ensuring consistent and readable visualization across different system parameters and window sizes.    
    
All parameters passed to the renderer are validated at run time using `throw` statements, preventing invalid configurations.

#### I/O
[...]

#### Main
[...]

---

## Input–Output
[...]

---

## Compilation
> [!NOTE]
> Make sure you have a C++20–compliant compiler.
> 
> If your compiler does not support C++20, please update it or use an alternative compiler that does.

- **Installation of SFML (graphics library)**

    Make sure you have **SFML** (version 2.6 or later) installed.
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

- **Creation of project directory**

    Next, create the `project_name/` directory (with the structure described above):
    ```bash
    $ mkdir project_name
    ```
    
    Alternatively, unzip `project_name.zip` (`project-main.zip` if downloaded directly from GitHub):
    ```bash
    $ unzip project_name.zip
    ...
    ```

    Later, navigate to the project directory:
    ```bash
    $ cd project_name
    ```

- **Creation of compile area**

    Create and configure the `build/` directory using the `CMakeLists.txt` file in the current directory (`.`):
    ```bash
    $ cmake -S . -B build -G"Ninja Multi-Config"
    ...
    ```

- **Compiling and testing**

    Now, compile the program in **Debug** mode and test it:
    ```bash
    $ cmake --build build --config Debug
    $ cmake --build build --config Debug --target test
    ```

    Then, repeat the same steps in **Release** mode:
    ```bash
    $ cmake --build build --config Release
    $ cmake --build build --config Release --target test
    ```

- **Running**

    Finally, run the executable compiled in **Debug** mode:
    ```bash
    $ ./build/Debug/project
    ...
    ```

    Insert the required parameters to start the simulation and view the graphical visualization.

    Possibly, run that compiled in **Release** mode:
    ```bash
    $ ./build/Release/project
    ...
    ```

    Again, insert the required parameters and try yourself!

---

## Results
[...]

---

## Testing
[...]

---

## Use of AI
[...]

---

## Author
**Tommaso Federici**  
Alma Mater Studiorum – University of Bologna  
Bachelor's Degree in Physics (Second Year)  
Email: tommaso.federici3@studio.unibo.it
