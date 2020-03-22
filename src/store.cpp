#include "store.h"

#include "semantic_error.h"

void StackLevel::registerName(const std::string& name, std::unique_ptr<ObjectData> objectData) {
  if (names.count(name) > 0) {
    throw SemanticError(name + " already exists in this context");
  }
  names[name] = std::move(objectData);
}

ObjectData* StackLevel::lookupName(const std::string& name) const {
  if (names.count(name) == 1) {
    return names.at(name).get();
  }
  return nullptr;
}

void Store::registerName(const std::string& name, std::unique_ptr<ObjectData> objectData) {
  stk.back().registerName(name, std::move(objectData));
}

const std::unique_ptr<Rvalue>& Store::getValue(const std::string& name) const {
  return getVariableData(name)->getValue();
}

void Store::setValue(const std::string& name, std::unique_ptr<Rvalue> value) {
  auto var = getVariableData(name);
  if (var->getValue()->getType() != value->getType()) {
    throw SemanticError("incompatible type for assignment to " + name);
  }
  var->setValue(std::move(value));
}

void Store::setValue(const std::string& name, std::vector<int> index, std::unique_ptr<Rvalue> value) {
  auto rv = getVariableData(name)->getValue().get();
  int last = index.back();
  index.pop_back();
  for (int i : index) {
    rv = dynamic_cast<ArrayRvalue*>(rv)->getValue()->at(i).get();
  }
  if (getArrayElementType(rv->getType()) != value->getType()) {
    throw SemanticError("incompatible type for assignment to " + name);
  }
  (*dynamic_cast<ArrayRvalue*>(rv)->getValue())[last] = std::move(value);
}

void Store::newLevel() { stk.emplace_back(); }

void Store::deleteLevel() { stk.pop_back(); }

VariableData* Store::getVariableData(const std::string& name) const {
  for (auto it = stk.rbegin(); it != stk.rend(); ++it) {
    ObjectData* data = it->lookupName(name);
    if (data != nullptr) {
      if (data->getType() != ObjectData::VARIABLE) {
        throw SemanticError(name + " is not a variable");
      }
      return dynamic_cast<VariableData*>(data);
    }
  }
  throw SemanticError(name + " is undefined in this context");
}

Store store;
