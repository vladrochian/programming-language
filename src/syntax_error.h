#ifndef BEAUTY_LANG_SYNTAX_ERROR_H
#define BEAUTY_LANG_SYNTAX_ERROR_H

#include <exception>
#include <sstream>
#include <string>

class SyntaxError : public std::exception {
  public:
    SyntaxError(int line, int col, std::string message) : line(line), col(col), message(std::move(message)) {}

    const char* what() const noexcept override {
        std::ostringstream os;
        os << "Syntax error (line " << line << ", col " << col << "): " << message;
        return os.str().c_str();
    }

  private:
    int line, col;
    std::string message;
};

#endif //BEAUTY_LANG_SYNTAX_ERROR_H
