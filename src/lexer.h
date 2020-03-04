#ifndef PROG_LANG_LEXER_H
#define PROG_LANG_LEXER_H

#include <string>
#include <vector>

#include "token.h"

class Lexer {
 public:
  static std::vector<Token> readfile(const std::string& fileName);

 private:
  static std::vector<Token> readLine(int lineIndex, const std::string& buffer);
  static int skipWhitespace(const std::string& buffer, std::string::const_iterator& it);
  static std::string getWord(const std::string& buffer, std::string::const_iterator& it);
  static double getNumber(const std::string& buffer, std::string::const_iterator& it);
  static std::string getString(const std::string& buffer, std::string::const_iterator& it);
  static std::string getOperator(const std::string& buffer, std::string::const_iterator& it);
};

#endif //PROG_LANG_LEXER_H
