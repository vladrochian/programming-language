cmake_minimum_required(VERSION 3.9)
project(beauty-lang)

set(CMAKE_CXX_STANDARD 17)

add_executable(beauty-lang src/main.cpp src/token.h src/lexer.cpp src/lexer.h src/token.cpp src/parser.h src/node.h src/primitive_types.h src/function.h src/object.h src/variable_store.h src/primitive_value.h)