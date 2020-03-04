cmake_minimum_required(VERSION 3.9)
project(prog-lang)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

add_executable(prog-lang src/main.cpp src/token.h src/lexer.cpp src/lexer.h src/token.cpp src/parser.h src/node.h src/types.h src/function.h src/store.h src/value.h src/operator.h src/parser.cpp src/keyword.h src/logger.h src/logger.cpp src/syntax_error.h src/node.cpp src/expression_parser.h src/expression_parser.cpp src/semantic_analyzer.h src/value.cpp src/semantic_analyzer.cpp src/semantic_error.h src/store.cpp src/vm.h src/vm.cpp src/runtime_error.h src/error.h)