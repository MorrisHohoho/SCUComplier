cmake_minimum_required(VERSION 3.24)
project(Lexer)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(Lexer
        include/FileController.h
        include/lexer.h
        include/lexertest.h
        src/FileController.cpp
        src/lexer.cpp
        src/lexertest.cpp
        src/main.cpp)
