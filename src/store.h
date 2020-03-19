#ifndef PROG_LANG_STORE_H
#define PROG_LANG_STORE_H

#include <exception>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "types.h"
#include "value.h"

class ObjectData {
 public:
  enum Type {
    VARIABLE,
    FUNCTION
  };

  virtual Type getType() const = 0;
};

class VariableData : public ObjectData {
 public:
  explicit VariableData(int type) : value() {
    switch (type) {
      case TYPE_BOOLEAN:
        value = std::make_unique<BooleanRvalue>(false);
        break;
      case TYPE_NUMBER:
        value = std::make_unique<NumberRvalue>(0.0);
        break;
      case TYPE_STRING:
        value = std::make_unique<StringRvalue>(std::string());
    }
  }

  Type getType() const override { return VARIABLE; }

  const std::unique_ptr<Rvalue>& getValue() const { return value; }

  void setValue(std::unique_ptr<Rvalue> v) { this->value = std::move(v); }

 private:
  std::unique_ptr<Rvalue> value;
};

class StackLevel {
 public:
  void registerName(const std::string& name, std::unique_ptr<ObjectData> objectData);
  ObjectData* lookupName(const std::string& name) const;

 private:
  std::map<std::string, std::unique_ptr<ObjectData>> names;
};

class Store {
 public:
  void registerName(const std::string& name, std::unique_ptr<ObjectData> objectData);
  const std::unique_ptr<Rvalue>& getValue(const std::string& name) const;
  void setValue(const std::string& name, std::unique_ptr<Rvalue> value);
  void newLevel();
  void deleteLevel();

 private:
  VariableData* getVariableData(const std::string& name) const;
  std::vector<StackLevel> stk;
};

extern Store store;

#endif //PROG_LANG_STORE_H
