cmake_minimum_required(VERSION 3.20)

project(wonin LANGUAGES CXX)

add_library(wonin
    STATIC
        src/log.cpp
        src/utils.cpp
        src/token.cpp
        src/scanner.cpp
        src/parser.cpp
        src/visitor.cpp
        src/interpreter.cpp
        src/wonin.cpp
)

target_compile_features(wonin
    PUBLIC
        cxx_std_20
)

target_include_directories(wonin
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if(WIN32)
    target_compile_definitions(wonin
        PUBLIC
            WONIN_PLATFORM_WINDOWS)
endif()
