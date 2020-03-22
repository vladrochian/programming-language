#include "vm.h"

#include <cmath>
#include <iostream>

#include "runtime_error.h"
#include "store.h"

std::pair<bool, std::unique_ptr<Value>> VirtualMachine::run(Node* node) {
  if (node->getType() == Node::BLOCK) {
    auto blockNode = dynamic_cast<BlockNode*>(node);
    store.newLevel();
    for (const auto& it : blockNode->getContent()) {
      auto ret = run(it.get());
      if (ret.first) {
        store.deleteLevel();
        return ret;
      }
    }
    store.deleteLevel();
  } else if (node->getType() == Node::STANDALONE_EXPRESSION) {
    evalExp(dynamic_cast<StandaloneExpressionNode*>(node)->getExpression().get());
  } else if (node->getType() == Node::RETURN_INSTRUCTION) {
    auto retNode = dynamic_cast<ReturnInstructionNode*>(node);
    if (!retNode->getExpression()) {
      return std::make_pair(true, nullptr);
    }
    auto retExprU = evalExp(retNode->getExpression().get());
    auto retExpr = retExprU->getRvalue();
    int type = retExpr->getType();
    std::unique_ptr<Value> nv;
    if (type == TYPE_BOOLEAN) {
      nv = std::make_unique<BooleanRvalue>(dynamic_cast<const BooleanRvalue*>(retExpr)->getValue());
    } else if (type == TYPE_NUMBER) {
      nv = std::make_unique<NumberRvalue>(dynamic_cast<const NumberRvalue*>(retExpr)->getValue());
    } else if (type == TYPE_STRING) {
      nv = std::make_unique<StringRvalue>(dynamic_cast<const StringRvalue*>(retExpr)->getValue());
    } else {
      nv = std::make_unique<ArrayRvalue>(*dynamic_cast<const ArrayRvalue*>(retExpr));
    }
    return std::make_pair(true, std::move(nv));
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
        if (stringValue == "true" || stringValue == "TRUE" || stringValue == "1" || stringValue == "t" ||
            stringValue == "T") {
          booleanValue = true;
        } else if (stringValue == "false" || stringValue == "FALSE" || stringValue == "0" || stringValue == "f" ||
                   stringValue == "F") {
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
      if (isTypeList(exprType)) {
        exprRet = std::make_unique<ArrayRvalue>(type,
            dynamic_cast<const ListRvalue*>(exprRet->getRvalue())->getValue());
      }
    }
    store.registerName(varDecNode->getVariableName(), std::make_unique<VariableData>(type, std::move(exprRet)));
  } else if (node->getType() == Node::FUNCTION_DEFINITION) {
    auto fncDefNode = dynamic_cast<FunctionDefinitionNode*>(node);
    store.registerName(fncDefNode->getFunctionName(), std::make_unique<FunctionData>(
        fncDefNode->getArguments(),
        fncDefNode->getReturnType(),
        fncDefNode->getBlock()
    ));
  } else if (node->getType() == Node::IF_STATEMENT) {
    auto ifNode = dynamic_cast<IfNode*>(node);
    if (getBooleanValue(evalExp(ifNode->getCondition().get()))) {
      auto ret = run(ifNode->getThenBlock().get());
      if (ret.first) {
        return std::move(ret);
      }
    } else if (ifNode->getElseBlock() != nullptr) {
      auto ret = run(ifNode->getElseBlock().get());
      if (ret.first) {
        return std::move(ret);
      }
    }
  } else if (node->getType() == Node::WHILE_STATEMENT) {
    auto whileNode = dynamic_cast<WhileNode*>(node);
    while (getBooleanValue(evalExp(whileNode->getCondition().get()))) {
      auto ret = run(whileNode->getBlock().get());
      if (ret.first) {
        return std::move(ret);
      }
    }
  } else if (node->getType() == Node::FOR_STATEMENT) {
    auto forNode = dynamic_cast<ForNode*>(node);
    auto range = evalExp(forNode->getRangeExpression().get());
    if (range->getType() == TYPE_STRING) {
      auto s = dynamic_cast<const StringRvalue*>(range->getRvalue())->getValue();
      for (char c : s) {
        std::string cs;
        cs += c;
        store.newLevel();
        store.registerName(forNode->getIterName(),
            std::make_unique<VariableData>(TYPE_STRING, std::make_unique<StringRvalue>(cs)));
        auto ret = run(forNode->getBlock().get());
        store.deleteLevel();
        if (ret.first) {
          return std::move(ret);
        }
      }
    } else {
      const auto& arr = isTypeArray(range->getType())
                        ? *dynamic_cast<const ArrayRvalue*>(range->getRvalue())->getValue()
                        : dynamic_cast<const ListRvalue*>(range->getRvalue())->getValue();
      for (const auto& it : arr) {
        auto rv = it->getRvalue();
        int type = rv->getType();
        std::unique_ptr<Value> elem;
        if (type == TYPE_BOOLEAN) {
          elem = std::make_unique<BooleanRvalue>(dynamic_cast<const BooleanRvalue*>(rv)->getValue());
        } else if (type == TYPE_NUMBER) {
          elem = std::make_unique<NumberRvalue>(dynamic_cast<const NumberRvalue*>(rv)->getValue());
        } else if (type == TYPE_STRING) {
          elem = std::make_unique<StringRvalue>(dynamic_cast<const StringRvalue*>(rv)->getValue());
        } else {
          elem = std::make_unique<ArrayRvalue>(*dynamic_cast<const ArrayRvalue*>(rv));
        }
        store.newLevel();
        store.registerName(forNode->getIterName(),
            std::make_unique<VariableData>(getArrayElementType(range->getType()), std::move(elem)));
        run(forNode->getBlock().get());
        store.deleteLevel();
      }
    }
  }
  return std::make_pair(false, std::unique_ptr<Value>(nullptr));
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
  if (node->getType() == Node::LIST_VALUE) {
    std::vector<std::shared_ptr<Rvalue>> v;
    int lt = TYPE_NONE;
    for (const auto& elem : dynamic_cast<ListValueNode*>(node)->getElements()) {
      auto er = evalExp(elem.get());
      auto expRes = er->getRvalue();
      int type = expRes->getType();
      if (lt == TYPE_NONE) {
        lt = type;
      } else if (lt != type) {
        lt = TYPE_MIXED;
      }
      if (type == TYPE_BOOLEAN) {
        v.emplace_back(std::make_shared<BooleanRvalue>(dynamic_cast<const BooleanRvalue*>(expRes)->getValue()));
      } else if (type == TYPE_NUMBER) {
        v.emplace_back(std::make_shared<NumberRvalue>(dynamic_cast<const NumberRvalue*>(expRes)->getValue()));
      } else if (type == TYPE_STRING) {
        v.emplace_back(std::make_shared<StringRvalue>(dynamic_cast<const StringRvalue*>(expRes)->getValue()));
      } else if (isTypeArray(type)) {
        v.emplace_back(std::make_shared<ArrayRvalue>(*dynamic_cast<const ArrayRvalue*>(expRes)));
      }
    }
    return std::make_unique<ListRvalue>(TYPE_LIST(lt), std::move(v));
  }
  if (node->getType() == Node::VARIABLE) {
    return std::make_unique<Lvalue>(dynamic_cast<VariableNode*>(node)->getName());
  }
  if (node->getType() == Node::FUNCTION_CALL) {
    auto fncNode = dynamic_cast<FunctionCallNode*>(node);
    auto fncData = store.getFunctionData(fncNode->getFunctionName());
    int argc = fncData->getArguments().size();
    store.newLevel();
    for (int i = 0; i < argc; ++i) {
      auto expr = fncNode->getArguments()[i].get();
      auto argv = evalExp(expr);
      store.registerName(
          fncData->getArguments()[i].first,
          std::make_unique<VariableData>(fncData->getArguments()[i].second, std::move(argv))
      );
    }
    auto ret = run(fncData->getBlock().get());
    store.deleteLevel();
    if (fncData->getReturnType() != TYPE_NONE && !ret.first) {
      throw RuntimeError("non-void function finished execution without returning any value");
    }
    return std::move(ret.second);
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
      } else if (isTypeArray(ls->getType())) {
        if (isTypeArray(rs->getType())) {
          dynamic_cast<Lvalue*>(ls.get())->setValue(
              std::make_unique<ArrayRvalue>(*dynamic_cast<const ArrayRvalue*>(rs->getRvalue()))
          );
        } else {
          dynamic_cast<Lvalue*>(ls.get())->setValue(
              std::make_unique<ArrayRvalue>(TYPE_ARRAY(getListElementType(rs->getType())),
                  dynamic_cast<const ListRvalue*>(rs->getRvalue())->getValue())
          );
        }
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
    case BinaryOperatorNode::INDEX:
      auto i = getNumberValue(rs);
      int ii = i;
      if (ii != i) {
        throw RuntimeError("non-integer number used as index");
      }
      if (ls->getType() == TYPE_STRING) {
        auto s = getStringValue(ls);
        if (ii < 0) {
          ii += static_cast<int>(s.size());
        }
        if (ii < 0 || ii >= static_cast<int>(s.size())) {
          throw RuntimeError("string index out of bounds");
        }
        std::string ans;
        ans += s[ii];
        return std::make_unique<StringRvalue>(ans);
      } else {
        auto arr = dynamic_cast<const ArrayRvalue*>(ls->getRvalue());
        int arrSize = static_cast<int>(arr->getValue()->size());
        if (ii < 0) {
          ii += arrSize;
        }
        if (ii < 0 || ii >= arrSize) {
          throw RuntimeError("array index out of bounds");
        }
        if (ls->getMemoryClass() == Value::LVALUE) {
          return std::make_unique<ElementLvalue>(dynamic_cast<const Lvalue*>(ls.get())->name, std::vector<int>(1, ii));
          // TODO: make it work for nested arrays
        }
        const auto& elem = dynamic_cast<ArrayRvalue*>(ls.get())->getValue()->at(ii);
        if (elem->getType() == TYPE_BOOLEAN) {
          return std::make_unique<BooleanRvalue>(dynamic_cast<BooleanRvalue*>(elem.get())->getValue());
        }
        if (elem->getType() == TYPE_NUMBER) {
          return std::make_unique<NumberRvalue>(dynamic_cast<NumberRvalue*>(elem.get())->getValue());
        }
        if (elem->getType() == TYPE_STRING) {
          return std::make_unique<StringRvalue>(dynamic_cast<StringRvalue*>(elem.get())->getValue());
        }
        if (isTypeArray(elem->getType())) {
          return std::make_unique<ArrayRvalue>(*dynamic_cast<ArrayRvalue*>(elem.get()));
        }
      }
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
