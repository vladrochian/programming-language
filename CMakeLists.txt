cmake_minimum_required(VERSION 3.9)
project(beauty-lang)

set(CMAKE_CXX_STANDARD 17)

add_executable(beauty-lang src/main.cpp src/token.h src/lexer.cpp src/lexer.h)