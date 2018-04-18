#ifndef BEAUTY_LANG_LEXER_H
#define BEAUTY_LANG_LEXER_H

#include <vector>
#include <fstream>
#include "token.h"

class Lexer {
  public:
    static std::vector<Token> parse(std::string buffer);

  private:
    Token *getNext(std::ifstream &file);

    Token *getID(std::ifstream &file, char &c);

    void skipWhitespaces(std::ifstream &file, char &c);

};

#endif //BEAUTY_LANG_LEXER_H
