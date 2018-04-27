#ifndef BEAUTY_LANG_OBJECT_H
#define BEAUTY_LANG_OBJECT_H

#include <string>

virtual class Object {
  public:
    virtual std::string getType() {
        return "";
    }
};

#endif //BEAUTY_LANG_OBJECT_H
