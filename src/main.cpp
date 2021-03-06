#include <fstream>
#include <iostream>
#include <string>

#include "error.h"
#include "expression_parser.h"
#include "lexer.h"
#include "parser.h"
#include "semantic_analyzer.h"
#include "vm.h"

void initialize() {
  initializeKeywordMapping();
  initializeOperatorTokenMapping();
  ExpressionParser::initializeData();
}

int main(int argc, char** argv) {
  initialize();
  if (argc < 2) {
    std::cout << "Please specify a source file as argument.\n";
    return 0;
  }
  std::string sourceFile = argv[1];
  if (!std::ifstream(sourceFile)) {
    std::cout << "Error: can not open source file.\n";
    return 0;
  }
  try {
    auto tokenList = Lexer::readfile(sourceFile);
    auto fileTree = Parser::parseFile(tokenList);
    SemanticAnalyzer::analyze(fileTree.get());
    VirtualMachine::run(fileTree.get());
  } catch (Error& e) {
    std::cout << e.toString() << "\n";
  }
  return 0;
}