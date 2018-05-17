#include "store.h"

#include "semantic_error.h"

VariableData::VariableData(PrimitiveType type) : type(type) {}

ObjectData::Type VariableData::getType() const { return VARIABLE; }

PrimitiveType VariableData::getVariableType() const { return type; }

bool VariableData::getBooleanValue() const { return booleanValue; }

double VariableData::getNumberValue() const { return numberValue; }

std::string VariableData::getStringValue() const { return stringValue; }

void VariableData::setValue(bool value) { booleanValue = value; }

void VariableData::setValue(double value) { numberValue = value; }

void VariableData::setValue(const std::string& value) { stringValue = value; }

void StackLevel::registerName(const std::string& name, const ObjectData& objectData) {
    names[name] = objectData;
}

ObjectData* StackLevel::lookupName(const std::string& name) const {
    if (names.count(name) == 1) {
        return &names[name];
    }
    return nullptr;
}

void Store::registerName(const std::string& name, const ObjectData& objectData) {
    stk.back().registerName(name, objectData);
}

Rvalue Store::getValue(const std::string& name) const {
    auto var = getVariableData(name);
    switch (var->getVariableType()) {
        case TYPE_BOOLEAN:
            return Rvalue(var->getBooleanValue());
        case TYPE_NUMBER:
            return Rvalue(var->getNumberValue());
        case TYPE_STRING:
            return Rvalue(var->getStringValue());
    }
}

void Store::setValue(const std::string& name, const Value& value) {
    auto var = getVariableData(name);
    if (var->getVariableType() != value.getType()) {
        throw SemanticError("incompatible type for assignment to " + name);
    }
    switch (value.getType()) {
        case TYPE_BOOLEAN:
            var->setValue(value.getBooleanValue());
            break;
        case TYPE_NUMBER:
            var->setValue(value.getNumberValue());
            break;
        case TYPE_STRING:
            var->setValue(value.getStringValue());
            break;
    }
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
