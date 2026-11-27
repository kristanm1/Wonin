cmake_minimum_required(VERSION 3.25)

project(interpreter)

add_executable(interpreter
    src/main.cpp
)

target_link_libraries(interpreter
    PRIVATE
        wonin
)
