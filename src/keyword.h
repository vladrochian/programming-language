#ifndef PROG_LANG_KEYWORD_H
#define PROG_LANG_KEYWORD_H

#include <map>
#include <string>

enum Keyword {
  KEYWORD_IF,
  KEYWORD_ELSE,
  KEYWORD_WHILE,
  KEYWORD_FOR,
  KEYWORD_BOOLEAN,
  KEYWORD_NUMBER,
  KEYWORD_STRING,
  KEYWORD_DEF,
  KEYWORD_RETURN,
  KEYWORD_PRINT
};

void initializeKeywordMapping();
const std::map<std::string, Keyword>& keywordMap();

#endif //PROG_LANG_KEYWORD_H
