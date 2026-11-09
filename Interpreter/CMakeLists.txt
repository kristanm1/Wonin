cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Interpreter)

add_executable(Interpreter
    src/Log.cpp
    src/Scanner.cpp
    src/Parser.cpp
    src/Interpreter.cpp
    src/Main.cpp)
