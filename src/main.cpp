#include <iostream>
#include "lexer.h"
#include "logger.h"
#include "node.h"

int main(int argc, char **argv) {
    try {
        auto tokenList = Lexer::readfile("../test_source.txt");
        for (const auto& it : tokenList) {
            Logger::print(it);
        }
    } catch (std::exception& e) {
        std::cout << e.what() << "\n";
    }
    Node* node = new Plus(5, 4);
    if (node->getType() == Node::WHILE) {
        auto ifNode = dynamic_cast<Plus&>(*node);
        std::cout << ifNode.leftSon();
    }
    return 0;
}