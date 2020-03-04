#ifndef PROG_LANG_VALUE_H
#define PROG_LANG_VALUE_H

#include <string>
#include <map>

#include "node.h"
#include "types.h"

class Rvalue;

class Value {
 public:
  enum MemoryClass {
    LVALUE,
    RVALUE
  };
  virtual ~Value() = default;
  virtual MemoryClass getMemoryClass() const = 0;
  virtual PrimitiveType getType() const = 0;
  virtual const Rvalue* getRvalue() const = 0;
};

class Rvalue : public Value {
 public:
  MemoryClass getMemoryClass() const override { return RVALUE; };
  const Rvalue* getRvalue() const override { return this; }
};

class Lvalue : public Value {
 public:
  explicit Lvalue(std::string name) : name(std::move(name)) {}
  MemoryClass getMemoryClass() const override { return LVALUE; }
  PrimitiveType getType() const override;
  const Rvalue* getRvalue() const override;
  void setValue(std::unique_ptr<Rvalue> value) const;

 private:
  std::string name;
};

class BooleanRvalue : public Rvalue {
 public:
  explicit BooleanRvalue(bool value) : value(value) {}
  PrimitiveType getType() const override { return TYPE_BOOLEAN; }
  bool getValue() const { return value; }

 private:
  bool value;
};

class NumberRvalue : public Rvalue {
 public:
  explicit NumberRvalue(double value) : value(value) {}
  PrimitiveType getType() const override { return TYPE_NUMBER; }
  double getValue() const { return value; }

 private:
  double value;
};

class StringRvalue : public Rvalue {
 public:
  explicit StringRvalue(std::string value) : value(std::move(value)) {}
  PrimitiveType getType() const override { return TYPE_STRING; }
  std::string getValue() const { return value; }

 private:
  std::string value;
};

#endif //PROG_LANG_PRIMITIVE_VALUE_H
