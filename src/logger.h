#ifndef BEAUTY_LANG_LOGGER_H
#define BEAUTY_LANG_LOGGER_H

#include <string>

#include "keyword.h"
#include "operator.h"
#include "token.h"

class Logger {
  public:
    static void print(const Token& token, const std::string& separator = " ");

  private:
    static std::string toString(OperatorToken op);
    static std::string toString(Keyword keyword);
};

#endif //BEAUTY_LANG_LOGGER_H
