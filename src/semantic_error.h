#ifndef BEAUTY_LANG_SEMANTIC_ERROR_H
#define BEAUTY_LANG_SEMANTIC_ERROR_H

#include <exception>
#include <sstream>
#include <string>

class SemanticError : public std::exception {
  public:
    explicit SemanticError(std::string message) : message(std::move(message)) {}

    const char* what() const noexcept override {
        std::ostringstream os;
        os << "Semantic error: " << message;
        return os.str().c_str();
    }

  private:
    std::string message;
};

#endif //BEAUTY_LANG_SEMANTIC_ERROR_H
