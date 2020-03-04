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

  virtual Type getType() const {}
};

class VariableData : public ObjectData {
 public:
  explicit VariableData(PrimitiveType type);
  Type getType() const override;
  PrimitiveType getVariableType() const;
  bool getBooleanValue() const;
  double getNumberValue() const;
  std::string getStringValue() const;
  void setValue(bool value);
  void setValue(double value);
  void setValue(const std::string& value);

 private:
  PrimitiveType type;
  bool booleanValue;
  double numberValue;
  std::string stringValue;
};

class StackLevel {
 public:
  void registerName(const std::string& name, std::unique_ptr<ObjectData> objectData);
  ObjectData* lookupName(const std::string& name);

 private:
  std::map<std::string, std::unique_ptr<ObjectData>> names;
};

class Store {
 public:
  void registerName(const std::string& name, std::unique_ptr<ObjectData> objectData);
  Rvalue getValue(const std::string& name);
  void setValue(const std::string& name, std::unique_ptr<Value> value);
  void newLevel();
  void deleteLevel();

 private:
  VariableData* getVariableData(const std::string& name);
  std::vector<StackLevel> stk;
};

extern Store store;

#endif //PROG_LANG_STORE_H
