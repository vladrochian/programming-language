#include <iostream>

#include "lexer.h"
#include "logger.h"
#include "parser.h"
#include "semantic_analyzer.h"

int main(int argc, char **argv) {
    try {
        auto tokenList = Lexer::readfile("../test_source.txt");
        for (const auto& it : tokenList) {
            Logger::print(it);
        }
        std::cout << "\n";
        auto fileTree = Parser::parseFile(tokenList);
        Logger::print(fileTree);
        SemanticAnalyzer::analyze(fileTree);
    } catch (std::exception& e) {
        std::cout << e.what() << "\n";
    }
    return 0;
}