#ifndef PROG_LANG_VALUE_H
#define PROG_LANG_VALUE_H

#include <string>
#include <map>

#include "node.h"
#include "types.h"

class Value {
 public:
  enum MemoryClass {
    LVALUE,
    RVALUE
  };
  virtual ~Value() = default;
  virtual MemoryClass getMemoryClass() const = 0;
  virtual PrimitiveType getType() const = 0;
  virtual bool getBooleanValue() const = 0;
  virtual double getNumberValue() const = 0;
  virtual std::string getStringValue() const = 0;
};

class Rvalue : public Value {
 public:
  explicit Rvalue(ExpressionNode* node);
  explicit Rvalue(bool booleanValue);
  explicit Rvalue(double numberValue);
  explicit Rvalue(std::string stringValue);
  MemoryClass getMemoryClass() const override;
  PrimitiveType getType() const override;
  bool getBooleanValue() const override;
  double getNumberValue() const override;
  std::string getStringValue() const override;

 private:
  PrimitiveType type;
  bool booleanValue;
  double numberValue;
  std::string stringValue;
};

class Lvalue : public Value {
 public:
  explicit Lvalue(std::string name);
  MemoryClass getMemoryClass() const override;
  PrimitiveType getType() const override;
  bool getBooleanValue() const override;
  double getNumberValue() const override;
  std::string getStringValue() const override;
  void setValue(std::unique_ptr<Value> other);

 private:
  Rvalue getValue() const;
  std::string name;
};

#endif //PROG_LANG_PRIMITIVE_VALUE_H
