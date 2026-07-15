cmake_minimum_required(VERSION 4.2)
project(Erupt)

set(CMAKE_CXX_STANDARD 20)

add_executable(Erupt main.cpp
        include/erupt.h
        src/erupt.cpp
        include/window.h
        src/window.cpp
        src/stepManager.cpp
        include/stepManager.h)
