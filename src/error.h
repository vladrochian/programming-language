#ifndef PROG_LANG_ERROR_H
#define PROG_LANG_ERROR_H

#include <exception>
#include <string>

class Error : public std::exception {
  public:
    virtual std::string toString() const noexcept = 0;
};

#endif //PROG_LANG_ERROR_H
