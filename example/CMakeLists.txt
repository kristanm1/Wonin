cmake_minimum_required(VERSION 3.20)

project(woninterpreter)

add_executable(woninterpreter
    src/main.cpp
)

target_link_libraries(woninterpreter
    PRIVATE
        wonin
)
