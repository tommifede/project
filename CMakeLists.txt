cmake_minimum_required(VERSION 3.28)
project(project VERSION 0.1.0)
include(CTest)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# c++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# warning
string(APPEND CMAKE_CXX_FLAGS
      " -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion"
      " -Wshadow -Wimplicit-fallthrough -Wextra-semi -Wold-style-cast"
      " -fno-omit-frame-pointer")

# stl assertions
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  string(APPEND CMAKE_CXX_FLAGS " -D_GLIBCXX_ASSERTIONS")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  string(APPEND CMAKE_CXX_FLAGS " -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE")
endif()

# address sanitizer and undefined-behaviour sanitizer in Debug mode
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address,undefined")
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  string(APPEND CMAKE_CXX_FLAGS " -D_GLIBCXX_SANITIZE_STD_ALLOCATOR")
endif()
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined")

# sfml
find_package(SFML 2.6 COMPONENTS graphics REQUIRED)

# core library
add_library(core STATIC
    src/simulation.cpp
    src/renderer.cpp
    io/input.cpp
    io/output.cpp
)
target_include_directories(core PUBLIC
    include
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(core PUBLIC sfml-graphics)

# main executable named "project"
add_executable(project src/main.cpp)
target_link_libraries(project PRIVATE core)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/font.ttf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# test (to disable test, type command -DBUILD_TESTING=off)
if (BUILD_TESTING)

  # simulation test executable named "simulation_test"
  add_executable(simulation_test test/simulation_test.cpp)
  target_link_libraries(simulation_test PRIVATE core)
  add_test(NAME simulation_test COMMAND simulation_test)

  # renderer test executable named "renderer_test"
  add_executable(renderer_test test/renderer_test.cpp)
  target_link_libraries(renderer_test PRIVATE core)
  add_test(NAME renderer_test COMMAND renderer_test)

endif()
