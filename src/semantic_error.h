#ifndef BEAUTY_LANG_SEMANTIC_ERROR_H
#define BEAUTY_LANG_SEMANTIC_ERROR_H

#include "error.h"

class SemanticError : public Error {
  public:
    explicit SemanticError(std::string message) : message(std::move(message)) {}

    std::string toString() const noexcept override {
        return "Semantic error: " + message;
    }

    const char* what() const noexcept override {
        return toString().c_str();
    }

  private:
    std::string message;
};

#endif //BEAUTY_LANG_SEMANTIC_ERROR_H
