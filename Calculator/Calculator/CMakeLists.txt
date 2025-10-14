cmake_minimum_required(VERSION 3.11)
project(Calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(Calculator
    main.cpp
    Calculator.cpp
    loader.cpp
)
