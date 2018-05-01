#ifndef BEAUTY_LANG_LEXER_H
#define BEAUTY_LANG_LEXER_H

#include <string>
#include <vector>

#include "token.h"

class Lexer {
  public:
    static std::vector<Token> parsefile(const std::string& fileName);

  private:
    static std::vector<Token> parseLine(int lineIndex, const std::string& buffer);
    static int skipWhitespace(const std::string& buffer, std::string::const_iterator& it);
};

#endif //BEAUTY_LANG_LEXER_H
