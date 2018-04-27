#ifndef BEAUTY_LANG_VARIABLE_STORE_H
#define BEAUTY_LANG_VARIABLE_STORE_H

#include <map>
#include <string>

class VariableStore {
  public:
    bool lookupName(const std::string& name) {
        return store.count(name) > 0;
    }

  private:
    std::map<std::string, std::pair<std::string, std::string>> store;
};

#endif //BEAUTY_LANG_VARIABLE_STORE_H
