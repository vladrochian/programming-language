cmake_minimum_required(VERSION 3.9)
project(beauty-lang)

set(CMAKE_CXX_STANDARD 17)

add_executable(beauty-lang src/main.cpp src/token.h src/lexer.cpp src/lexer.h src/token.cpp src/parser.h src/node.h src/types.h src/function.h src/object.h src/variable_store.h src/value.h src/operator.h src/parser.cpp src/operator_precedence.h src/keyword.h src/logger.h src/logger.cpp src/syntax_error.h src/node.cpp src/expression_parser.h src/expression_parser.cpp)