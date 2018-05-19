#include <iostream>

#include "error.h"
#include "lexer.h"
#include "parser.h"
#include "semantic_analyzer.h"
#include "vm.h"

int main(int argc, char **argv) {
    try {
        auto tokenList = Lexer::readfile("../test_source.txt");
        auto fileTree = Parser::parseFile(tokenList);
        SemanticAnalyzer::analyze(fileTree);
        VirtualMachine::run(fileTree);
    } catch (Error& e) {
        std::cout << e.toString() << "\n";
    }
    return 0;
}