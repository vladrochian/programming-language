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
  VariableData(int type, std::unique_ptr<Value> value) : value() {
    const auto v = value ? value->getRvalue() : nullptr;
    if (type == TYPE_BOOLEAN) {
      this->value = std::make_unique<BooleanRvalue>(value ? dynamic_cast<const BooleanRvalue*>(v)->getValue() : false);
    } else if (type == TYPE_NUMBER) {
      this->value = std::make_unique<NumberRvalue>(value ? dynamic_cast<const NumberRvalue*>(v)->getValue() : 0.0);
    } else if (type == TYPE_STRING) {
      this->value = std::make_unique<StringRvalue>(value ? dynamic_cast<const StringRvalue*>(v)->getValue() : std::string());
    } else if (isTypeArray(type)) {
      this->value = value ? std::make_unique<ArrayRvalue>(*dynamic_cast<const ArrayRvalue*>(v))
                          : std::make_unique<ArrayRvalue>(type);
    } else if (isTypeObj(type)) {

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
  void setValue(const std::string& name, std::vector<int> index, std::unique_ptr<Rvalue> value);
  void newLevel();
  void deleteLevel();

 private:
  VariableData* getVariableData(const std::string& name) const;
  std::vector<StackLevel> stk;
};

extern Store store;

#endif //PROG_LANG_STORE_H
