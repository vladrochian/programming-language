#include "vm.h"

#include <cmath>
#include <iostream>

#include "runtime_error.h"
#include "store.h"

void VirtualMachine::run(Node* node) {
  if (node->getType() == Node::BLOCK) {
    auto blockNode = dynamic_cast<BlockNode*>(node);
    store.newLevel();
    for (const auto& it : blockNode->getContent()) {
      run(it.get());
    }
    store.deleteLevel();
  } else if (node->getType() == Node::STANDALONE_EXPRESSION) {
    evalExp(dynamic_cast<StandaloneExpressionNode*>(node)->getExpression().get());
  } else if (node->getType() == Node::RETURN_INSTRUCTION) {
    // TODO: functions
    evalExp(dynamic_cast<ReturnInstructionNode*>(node)->getExpression().get());
  } else if (node->getType() == Node::PRINT_INSTRUCTION) {
    auto value = evalExp(dynamic_cast<PrintInstructionNode*>(node)->getExpression().get());
    switch (value->getType()) {
      case TYPE_BOOLEAN:
        std::cout << (getBooleanValue(value) ? "true\n" : "false\n");
        break;
      case TYPE_NUMBER:
        std::cout << getNumberValue(value) << "\n";
        break;
      case TYPE_STRING:
        std::cout << getStringValue(value) << "\n";
        break;
    }
  } else if (node->getType() == Node::READ_INSTRUCTION) {
    auto value = evalExp(dynamic_cast<ReadInstructionNode*>(node)->getExpression().get());
    bool booleanValue;
    double numberValue;
    std::string stringValue;
    switch (value->getType()) {
      case TYPE_BOOLEAN:
        std::cin >> stringValue;
        if (stringValue == "true" || stringValue == "TRUE" || stringValue == "1" || stringValue == "t" || stringValue == "T") {
          booleanValue = true;
        } else if (stringValue == "false" || stringValue == "FALSE" || stringValue == "0" || stringValue == "f" || stringValue == "F") {
          booleanValue = false;
        } else {
          throw RuntimeError("invalid input for boolean type");
        }
        dynamic_cast<Lvalue*>(value.get())->setValue(std::make_unique<BooleanRvalue>(booleanValue));
        break;
      case TYPE_NUMBER:
        if (!(std::cin >> numberValue)) {
          throw RuntimeError("invalid input for number type");
        }
        dynamic_cast<Lvalue*>(value.get())->setValue(std::make_unique<NumberRvalue>(numberValue));
        break;
      case TYPE_STRING:
        std::cin >> stringValue;
        dynamic_cast<Lvalue*>(value.get())->setValue(std::make_unique<StringRvalue>(stringValue));
        break;
    }
  } else if (node->getType() == Node::VARIABLE_DECLARATION) {
    auto varDecNode = dynamic_cast<VariableDeclarationNode*>(node);
    int type = varDecNode->getVariableType();
    std::unique_ptr<Value> exprRet;
    if (varDecNode->getInitializer()) {
      exprRet = evalExp(varDecNode->getInitializer().get());
      int exprType = exprRet->getType();
      if (type == TYPE_NONE) {
        if (isTypeList(exprType)) {
          type = TYPE_ARRAY(getListElementType(exprType));
        } else {
          type = exprType;
        }
      }
    }
    store.registerName(varDecNode->getVariableName(), std::make_unique<VariableData>(type, std::move(exprRet)));
  } else if (node->getType() == Node::IF_STATEMENT) {
    auto ifNode = dynamic_cast<IfNode*>(node);
    if (getBooleanValue(evalExp(ifNode->getCondition().get()))) {
      run(ifNode->getThenBlock().get());
    } else if (ifNode->getElseBlock() != nullptr) {
      run(ifNode->getElseBlock().get());
    }
  } else if (node->getType() == Node::WHILE_STATEMENT) {
    auto whileNode = dynamic_cast<WhileNode*>(node);
    while (getBooleanValue(evalExp(whileNode->getCondition().get()))) {
      run(whileNode->getBlock().get());
    }
  }
}

std::unique_ptr<Value> VirtualMachine::evalExp(ExpressionNode* node) {
  if (node->getType() == Node::BOOLEAN_VALUE) {
    return std::make_unique<BooleanRvalue>(dynamic_cast<BooleanValueNode*>(node)->getValue());
  }
  if (node->getType() == Node::NUMBER_VALUE) {
    return std::make_unique<NumberRvalue>(dynamic_cast<NumberValueNode*>(node)->getValue());
  }
  if (node->getType() == Node::STRING_VALUE) {
    return std::make_unique<StringRvalue>(dynamic_cast<StringValueNode*>(node)->getValue());
  }
  if (node->getType() == Node::VARIABLE) {
    return std::make_unique<Lvalue>(dynamic_cast<VariableNode*>(node)->getName());
  }
  if (node->getType() == Node::UNARY_OPERATOR) {
    auto unOpNode = dynamic_cast<UnaryOperatorNode*>(node);
    switch (unOpNode->getOperator()) {
      case UnaryOperatorNode::PLUS:
        return evalExp(unOpNode->getOperand().get());
      case UnaryOperatorNode::MINUS:
        return std::make_unique<NumberRvalue>(-getNumberValue(evalExp(unOpNode->getOperand().get())));
      case UnaryOperatorNode::NOT:
        return std::make_unique<BooleanRvalue>(!getBooleanValue(evalExp(unOpNode->getOperand().get())));
    }
  }
  auto binOpNode = dynamic_cast<BinaryOperatorNode*>(node);
  auto ls = evalExp(binOpNode->getLeftOperand().get());
  auto rs = evalExp(binOpNode->getRightOperand().get());
  switch (binOpNode->getOperator()) {
    case BinaryOperatorNode::ADD:
      if (ls->getType() == TYPE_NUMBER) {
        return std::make_unique<NumberRvalue>(getNumberValue(ls) + getNumberValue(rs));
      }
      return std::make_unique<StringRvalue>(getStringValue(ls) + getStringValue(rs));
    case BinaryOperatorNode::SUBTRACT:
      return std::make_unique<NumberRvalue>(getNumberValue(ls) - getNumberValue(rs));
    case BinaryOperatorNode::MULTIPLY:
      return std::make_unique<NumberRvalue>(getNumberValue(ls) * getNumberValue(rs));
    case BinaryOperatorNode::DIVIDE:
      if (getNumberValue(rs) == 0.0) {
        throw RuntimeError("division by 0");
      }
      return std::make_unique<NumberRvalue>(getNumberValue(ls) / getNumberValue(rs));
    case BinaryOperatorNode::REMAINDER:
      return std::make_unique<NumberRvalue>(getNumberValue(ls) -
                                            std::floor(getNumberValue(ls) / getNumberValue(rs)) * getNumberValue(rs));
    case BinaryOperatorNode::OR:
      return std::make_unique<BooleanRvalue>(getBooleanValue(ls) || getBooleanValue(rs));
    case BinaryOperatorNode::AND:
      return std::make_unique<BooleanRvalue>(getBooleanValue(ls) && getBooleanValue(rs));
    case BinaryOperatorNode::ASSIGN:
      if (ls->getType() == TYPE_BOOLEAN) {
        dynamic_cast<Lvalue*>(ls.get())->setValue(std::make_unique<BooleanRvalue>(getBooleanValue(rs)));
      } else if (ls->getType() == TYPE_NUMBER) {
        dynamic_cast<Lvalue*>(ls.get())->setValue(std::make_unique<NumberRvalue>(getNumberValue(rs)));
      } else if (ls->getType() == TYPE_STRING) {
        dynamic_cast<Lvalue*>(ls.get())->setValue(std::make_unique<StringRvalue>(getStringValue(rs)));
      }
      return ls;
    case BinaryOperatorNode::ADD_ASSIGN:
      if (ls->getType() == TYPE_NUMBER) {
        dynamic_cast<Lvalue*>(ls.get())->setValue(
            std::make_unique<NumberRvalue>(getNumberValue(ls) + getNumberValue(rs)));
      } else {
        dynamic_cast<Lvalue*>(ls.get())->setValue(
            std::make_unique<StringRvalue>(getStringValue(ls) + getStringValue(rs)));
      }
      return ls;
    case BinaryOperatorNode::SUBTRACT_ASSIGN:
      dynamic_cast<Lvalue*>(ls.get())->setValue(
          std::make_unique<NumberRvalue>(getNumberValue(ls) - getNumberValue(rs)));
      return ls;
    case BinaryOperatorNode::MULTIPLY_ASSIGN:
      dynamic_cast<Lvalue*>(ls.get())->setValue(
          std::make_unique<NumberRvalue>(getNumberValue(ls) * getNumberValue(rs)));
      return ls;
    case BinaryOperatorNode::DIVIDE_ASSIGN:
      if (getNumberValue(rs) == 0.0) {
        throw RuntimeError("division by 0");
      }
      dynamic_cast<Lvalue*>(ls.get())->setValue(
          std::make_unique<NumberRvalue>(getNumberValue(ls) / getNumberValue(rs)));
      return ls;
    case BinaryOperatorNode::REMAINDER_ASSIGN:
      dynamic_cast<Lvalue*>(ls.get())->setValue(std::make_unique<NumberRvalue>(getNumberValue(ls) -
                                                                               std::floor(getNumberValue(ls) /
                                                                                          getNumberValue(rs)) *
                                                                               getNumberValue(rs)));
      return ls;
    case BinaryOperatorNode::OR_ASSIGN:
      dynamic_cast<Lvalue*>(ls.get())->setValue(
          std::make_unique<BooleanRvalue>(getBooleanValue(ls) || getBooleanValue(rs)));
      return ls;
    case BinaryOperatorNode::AND_ASSIGN:
      dynamic_cast<Lvalue*>(ls.get())->setValue(
          std::make_unique<BooleanRvalue>(getBooleanValue(ls) && getBooleanValue(rs)));
      return ls;
    case BinaryOperatorNode::EQUAL:
      switch (ls->getType()) {
        case TYPE_BOOLEAN:
          return std::make_unique<BooleanRvalue>(getBooleanValue(ls) == getBooleanValue(rs));
        case TYPE_NUMBER:
          return std::make_unique<BooleanRvalue>(getNumberValue(ls) == getNumberValue(rs));
        case TYPE_STRING:
          return std::make_unique<BooleanRvalue>(getStringValue(ls) == getStringValue(rs));
      }
    case BinaryOperatorNode::DIFFERENT:
      switch (ls->getType()) {
        case TYPE_BOOLEAN:
          return std::make_unique<BooleanRvalue>(getBooleanValue(ls) != getBooleanValue(rs));
        case TYPE_NUMBER:
          return std::make_unique<BooleanRvalue>(getNumberValue(ls) != getNumberValue(rs));
        case TYPE_STRING:
          return std::make_unique<BooleanRvalue>(getStringValue(ls) != getStringValue(rs));
      }
    case BinaryOperatorNode::LESS:
      if (ls->getType() == TYPE_NUMBER) {
        return std::make_unique<BooleanRvalue>(getNumberValue(ls) < getNumberValue(rs));
      }
      return std::make_unique<BooleanRvalue>(getStringValue(ls) < getStringValue(rs));
    case BinaryOperatorNode::GREATER:
      if (ls->getType() == TYPE_NUMBER) {
        return std::make_unique<BooleanRvalue>(getNumberValue(ls) > getNumberValue(rs));
      }
      return std::make_unique<BooleanRvalue>(getStringValue(ls) > getStringValue(rs));
    case BinaryOperatorNode::LESS_EQUAL:
      if (ls->getType() == TYPE_NUMBER) {
        return std::make_unique<BooleanRvalue>(getNumberValue(ls) <= getNumberValue(rs));
      }
      return std::make_unique<BooleanRvalue>(getStringValue(ls) <= getStringValue(rs));
    case BinaryOperatorNode::GREATER_EQUAL:
      if (ls->getType() == TYPE_NUMBER) {
        return std::make_unique<BooleanRvalue>(getNumberValue(ls) >= getNumberValue(rs));
      }
      return std::make_unique<BooleanRvalue>(getStringValue(ls) >= getStringValue(rs));
  }
}

bool VirtualMachine::getBooleanValue(const std::unique_ptr<Value>& value) {
  return dynamic_cast<const BooleanRvalue*>(value->getRvalue())->getValue();
}

double VirtualMachine::getNumberValue(const std::unique_ptr<Value>& value) {
  return dynamic_cast<const NumberRvalue*>(value->getRvalue())->getValue();
}

std::string VirtualMachine::getStringValue(const std::unique_ptr<Value>& value) {
  return dynamic_cast<const StringRvalue*>(value->getRvalue())->getValue();
}
