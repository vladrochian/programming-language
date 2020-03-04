#ifndef PROG_LANG_SYNTAX_ERROR_H
#define PROG_LANG_SYNTAX_ERROR_H

#include <sstream>

#include "error.h"

class SyntaxError : public Error {
  public:
    SyntaxError(int line, int col, std::string message) : line(line), col(col), message(std::move(message)) {}

    std::string toString() const noexcept override {
        std::ostringstream os;
        os << "Syntax error (line " << line << ", col " << col << "): " << message;
        return os.str();
    }

    const char* what() const noexcept override {
        return toString().c_str();
    }

  private:
    int line, col;
    std::string message;
};

#endif //PROG_LANG_SYNTAX_ERROR_H
