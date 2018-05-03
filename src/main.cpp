#include "lexer.h"
#include "logger.h"

int main(int argc, char **argv) {
    auto tokenList = Lexer::readfile("test_source.txt");
    for (const auto& it : tokenList) {
        Logger::print(it);
    }
    return 0;
}