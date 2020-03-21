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
  virtual int getType() const = 0;
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
  int getType() const override;
  const Rvalue* getRvalue() const override;
  virtual void setValue(std::unique_ptr<Rvalue> value) const;

  std::string name;
};

class ElementLvalue : public Lvalue {
 public:
  ElementLvalue(std::string name, std::vector<int> index) : Lvalue(std::move(name)), index(std::move(index)) {}
  const Rvalue* getRvalue() const override;
  void setValue(std::unique_ptr<Rvalue> value) const override;

 private:
  std::vector<int> index;
};

class BooleanRvalue : public Rvalue {
 public:
  explicit BooleanRvalue(bool value) : value(value) {}
  int getType() const override { return TYPE_BOOLEAN; }
  bool getValue() const { return value; }

 private:
  bool value;
};

class NumberRvalue : public Rvalue {
 public:
  explicit NumberRvalue(double value) : value(value) {}
  int getType() const override { return TYPE_NUMBER; }
  double getValue() const { return value; }

 private:
  double value;
};

class StringRvalue : public Rvalue {
 public:
  explicit StringRvalue(std::string value) : value(std::move(value)) {}
  int getType() const override { return TYPE_STRING; }
  std::string getValue() const { return value; }

 private:
  std::string value;
};

class ArrayRvalue : public Rvalue {
 public:
  explicit ArrayRvalue(int type) : value(std::make_shared<std::vector<std::shared_ptr<Rvalue>>>()), type(type) {}
  ArrayRvalue(int type, std::vector<std::shared_ptr<Rvalue>> v)
    : value(std::make_shared<std::vector<std::shared_ptr<Rvalue>>>(std::move(v))), type(type) {}
  ArrayRvalue(const ArrayRvalue& other) : type(other.type), value(other.value) {}
  int getType() const override { return type; }
  int getElementType() const { return getArrayElementType(type); }
  const std::shared_ptr<std::vector<std::shared_ptr<Rvalue>>>& getValue() const { return value; }

 private:
  int type;
  std::shared_ptr<std::vector<std::shared_ptr<Rvalue>>> value;
};

class ListRvalue : public Rvalue {
 public:
  explicit ListRvalue(int type, std::vector<std::shared_ptr<Rvalue>> value) : value(std::move(value)), type(type) {}
  int getType() const override { return type; }
  int getElementType() const { return getListElementType(type); }
  const std::vector<std::shared_ptr<Rvalue>>& getValue() const { return value; }

 private:
  int type;
  std::vector<std::shared_ptr<Rvalue>> value;
};

#endif //PROG_LANG_PRIMITIVE_VALUE_H
