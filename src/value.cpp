#include "value.h"

#include "store.h"

Rvalue::Rvalue(bool booleanValue) : type(TYPE_BOOLEAN), booleanValue(booleanValue) {}

Rvalue::Rvalue(double numberValue) : type(TYPE_NUMBER), numberValue(numberValue) {}

Rvalue::Rvalue(std::string stringValue) : type(TYPE_STRING), stringValue(std::move(stringValue)) {}

Rvalue::Rvalue(ExpressionNode* node) {
    switch (node->getType()) {
        case Node::BOOLEAN_VALUE:
            type = TYPE_BOOLEAN;
            booleanValue = dynamic_cast<BooleanValueNode*>(node)->getValue();
            break;
        case Node::NUMBER_VALUE:
            type = TYPE_NUMBER;
            numberValue = dynamic_cast<NumberValueNode*>(node)->getValue();
            break;
        case Node::STRING_VALUE:
            type = TYPE_STRING;
            stringValue = dynamic_cast<StringValueNode*>(node)->getValue();
            break;
        default:
            break;
    }
}

Value::MemoryClass Rvalue::getMemoryClass() const { return RVALUE; }

PrimitiveType Rvalue::getType() const { return type; }

bool Rvalue::getBooleanValue() const { return booleanValue; }

double Rvalue::getNumberValue() const { return numberValue; }

std::string Rvalue::getStringValue() const { return stringValue; }

Lvalue::Lvalue(const std::string& name) : name(name) {}

Value::MemoryClass Lvalue::getMemoryClass() const { return LVALUE; }

PrimitiveType Lvalue::getType() const { return getValue().getType(); }

bool Lvalue::getBooleanValue() const { return getValue().getBooleanValue(); }

double Lvalue::getNumberValue() const { return getValue().getNumberValue(); }

std::string Lvalue::getStringValue() const { return getValue().getStringValue(); }

void Lvalue::setValue(Value* other) { store.setValue(name, other); }

Rvalue Lvalue::getValue() const { return store.getValue(name); }
