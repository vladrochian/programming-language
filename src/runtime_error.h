#ifndef BEAUTY_LANG_RUNTIME_ERROR_H
#define BEAUTY_LANG_RUNTIME_ERROR_H

#include "error.h"

class RuntimeError : public Error {
  public:
    explicit RuntimeError(std::string message) : message(std::move(message)) {}

    std::string toString() const noexcept override {
        return "Runtime error: " + message;
    }

    const char* what() const noexcept override {
        return toString().c_str();
    }

  private:
    std::string message;
};

#endif //BEAUTY_LANG_RUNTIME_ERROR_H
