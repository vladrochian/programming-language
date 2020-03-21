#include "semantic_analyzer.h"

#include "semantic_error.h"
#include "store.h"

void SemanticAnalyzer::analyze(Node* node) {
  if (node->getType() == Node::BLOCK) {
    auto blockNode = dynamic_cast<BlockNode*>(node);
    store.newLevel();
    for (const auto& it : blockNode->getContent()) {
      analyze(it.get());
    }
    store.deleteLevel();
  } else if (node->getType() == Node::STANDALONE_EXPRESSION) {
    analyzeExpr(dynamic_cast<StandaloneExpressionNode*>(node)->getExpression().get());
  } else if (node->getType() == Node::RETURN_INSTRUCTION) {
    analyzeExpr(dynamic_cast<ReturnInstructionNode*>(node)->getExpression().get());
  } else if (node->getType() == Node::PRINT_INSTRUCTION) {
    auto exprToPrint = dynamic_cast<PrintInstructionNode*>(node)->getExpression().get();
    analyzeExpr(exprToPrint);
    int eType = getExpressionType(exprToPrint);
    if (eType != TYPE_BOOLEAN && eType != TYPE_NUMBER && eType != TYPE_STRING) {
      throw SemanticError("print statement only accepts primitive types");
    }
  } else if (node->getType() == Node::READ_INSTRUCTION) {
    auto exprToRead = dynamic_cast<ReadInstructionNode*>(node)->getExpression().get();
    analyzeExpr(exprToRead);
    int eType = getExpressionType(exprToRead);
    if (eType != TYPE_BOOLEAN && eType != TYPE_NUMBER && eType != TYPE_STRING) {
      throw SemanticError("read statement only accepts primitive types");
    }
    if (getExpressionMemoryClass(exprToRead) == Value::RVALUE) {
      throw SemanticError("rvalue as argument for read statement");
    }
  } else if (node->getType() == Node::VARIABLE_DECLARATION) {
    auto varDecNode = dynamic_cast<VariableDeclarationNode*>(node);
    int type = varDecNode->getVariableType();
    if (varDecNode->getInitializer()) {
      analyzeExpr(varDecNode->getInitializer().get());
      int exprType = getExpressionType(varDecNode->getInitializer().get());
      if (exprType == TYPE_NONE) {
        throw SemanticError("void expression passed as initializer");
      }
      if (type == TYPE_NONE) {
        if (isTypeList(exprType)) {
          if (getListElementType(exprType) == TYPE_NONE) {
            throw SemanticError("cannot deduce array type from an empty list");
          }
          if (getListElementType(exprType) == TYPE_MIXED) {
            throw SemanticError("multiple type list passed as initializer for array");
          }
          type = TYPE_ARRAY(getListElementType(exprType));
        } else {
          type = exprType;
        }
      } else if (isTypeArray(type) && isTypeList(exprType)) {
        if (getListElementType(exprType) == TYPE_MIXED) {
          throw SemanticError("multiple type list passed as initializer for array");
        }
        if (getListElementType(exprType) != TYPE_NONE && getArrayElementType(type) != getListElementType(exprType)) {
          throw SemanticError("array and initializer list types do not match");
        }
      } else if (isTypeObj(type) && isTypeList(exprType)) {
        // TODO: object init
      } else if (type != exprType) {
        throw SemanticError("initializer does not match variable type");
      }
    } else if (type == TYPE_NONE) {
      throw SemanticError("variable without type requires initializer");
    }
    store.registerName(varDecNode->getVariableName(), std::make_unique<VariableData>(type, nullptr));
  } else if (node->getType() == Node::IF_STATEMENT) {
    auto ifNode = dynamic_cast<IfNode*>(node);
    analyzeExpr(ifNode->getCondition().get());
    if (getExpressionType(ifNode->getCondition().get()) != TYPE_BOOLEAN) {
      throw SemanticError("if condition should have boolean type");
    }
    analyze(ifNode->getThenBlock().get());
    if (ifNode->getElseBlock() != nullptr) {
      analyze(ifNode->getElseBlock().get());
    }
  } else if (node->getType() == Node::WHILE_STATEMENT) {
    auto whileNode = dynamic_cast<WhileNode*>(node);
    analyzeExpr(whileNode->getCondition().get());
    if (getExpressionType(whileNode->getCondition().get()) != TYPE_BOOLEAN) {
      throw SemanticError("while condition should have boolean type");
    }
    analyze(whileNode->getBlock().get());
  } else if (node->getType() == Node::FOR_STATEMENT) {
    auto forNode = dynamic_cast<ForNode*>(node);
    auto range = forNode->getRangeExpression().get();
    analyzeExpr(range);
    int eType = getExpressionType(range);
    if (eType != TYPE_STRING && !isTypeArray(eType) && !isTypeList(eType)) {
      throw SemanticError("iteration can only be performed on strings and arrays");
    }
    if (isTypeList(eType) && getListElementType(eType) == TYPE_MIXED) {
      throw SemanticError("iteration can not be performed on mixed type lists");
    }
    store.newLevel();
    int elemType =
        eType == TYPE_STRING ? TYPE_STRING : isTypeArray(eType) ? getArrayElementType(eType) : getListElementType(
            eType);
    store.registerName(forNode->getIterName(), std::make_unique<VariableData>(elemType, nullptr));
    analyze(forNode->getBlock().get());
    store.deleteLevel();
  }
}

void SemanticAnalyzer::analyzeExpr(ExpressionNode* node) {
  getExpressionType(node);
  getExpressionMemoryClass(node);
}

int SemanticAnalyzer::getExpressionType(ExpressionNode* node) {
  auto unOpNode = dynamic_cast<UnaryOperatorNode*>(node);
  auto binOpNode = dynamic_cast<BinaryOperatorNode*>(node);
  switch (node->getType()) {
    case Node::BOOLEAN_VALUE:
      return TYPE_BOOLEAN;
    case Node::NUMBER_VALUE:
      return TYPE_NUMBER;
    case Node::STRING_VALUE:
      return TYPE_STRING;
    case Node::LIST_VALUE: {
      int lt = TYPE_NONE;
      for (const auto& element : dynamic_cast<ListValueNode*>(node)->getElements()) {
        int type = getExpressionType(element.get());
        if (type == TYPE_NONE) {
          throw SemanticError("void expressions are not allowed in lists");
        }
        if (lt == TYPE_NONE) {
          lt = type;
        } else if (type != lt) {
          lt = TYPE_MIXED;
        }
      }
      return TYPE_LIST(lt);
    }
    case Node::UNARY_OPERATOR:
      return getResultType(unOpNode->getOperator(), getExpressionType(unOpNode->getOperand().get()));
    case Node::BINARY_OPERATOR:
      return getResultType(binOpNode->getOperator(), getExpressionType(binOpNode->getLeftOperand().get()),
          getExpressionType(binOpNode->getRightOperand().get()));
    case Node::VARIABLE:
      return Lvalue(dynamic_cast<VariableNode*>(node)->getName()).getType();
    default:
      return TYPE_NONE;
  }
}

Value::MemoryClass SemanticAnalyzer::getExpressionMemoryClass(ExpressionNode* node) {
  auto unOpNode = dynamic_cast<UnaryOperatorNode*>(node);
  auto binOpNode = dynamic_cast<BinaryOperatorNode*>(node);
  switch (node->getType()) {
    case Node::BOOLEAN_VALUE:
    case Node::NUMBER_VALUE:
    case Node::STRING_VALUE:
      return Value::RVALUE;
    case Node::UNARY_OPERATOR:
      return getMemoryClass(unOpNode->getOperator(), getExpressionMemoryClass(unOpNode->getOperand().get()));
    case Node::BINARY_OPERATOR:
      return getMemoryClass(binOpNode->getOperator(), getExpressionMemoryClass(binOpNode->getLeftOperand().get()),
          getExpressionMemoryClass(binOpNode->getRightOperand().get()));
    case Node::VARIABLE:
      return Value::LVALUE;
    default:
      break;
  }
}

int SemanticAnalyzer::getResultType(UnaryOperatorNode::UnaryOperator op, int type) {
  if ((op == UnaryOperatorNode::NOT && type == TYPE_BOOLEAN) ||
      (op == UnaryOperatorNode::PLUS && type == TYPE_NUMBER) ||
      (op == UnaryOperatorNode::MINUS && type == TYPE_NUMBER)) {
    return type;
  }
  throw SemanticError("invalid operand");
}

int SemanticAnalyzer::getResultType(BinaryOperatorNode::BinaryOperator op, int lhs, int rhs) {
  switch (op) {
    case BinaryOperatorNode::ADD:
      if (lhs == TYPE_NUMBER && rhs == TYPE_NUMBER) return TYPE_NUMBER;
      if (lhs == TYPE_STRING && rhs == TYPE_STRING) return TYPE_STRING;
      break;
    case BinaryOperatorNode::SUBTRACT:
    case BinaryOperatorNode::MULTIPLY:
    case BinaryOperatorNode::DIVIDE:
    case BinaryOperatorNode::REMAINDER:
      if (lhs == TYPE_NUMBER && rhs == TYPE_NUMBER) return TYPE_NUMBER;
      break;
    case BinaryOperatorNode::OR:
    case BinaryOperatorNode::AND:
      if (lhs == TYPE_BOOLEAN && rhs == TYPE_BOOLEAN) return TYPE_BOOLEAN;
      break;
    case BinaryOperatorNode::ASSIGN:
      if (lhs == TYPE_BOOLEAN && rhs == TYPE_BOOLEAN) return TYPE_BOOLEAN;
      if (lhs == TYPE_NUMBER && rhs == TYPE_NUMBER) return TYPE_NUMBER;
      if (lhs == TYPE_STRING && rhs == TYPE_STRING) return TYPE_STRING;
      if (isTypeArray(lhs) && rhs == lhs) return lhs;
      if (isTypeArray(lhs) && isTypeList(rhs)
          && (getArrayElementType(lhs) == getListElementType(rhs) || getListElementType(rhs) == TYPE_NONE)) {
        return lhs;
      }
      break;
    case BinaryOperatorNode::ADD_ASSIGN:
      if (lhs == TYPE_NUMBER && rhs == TYPE_NUMBER) return TYPE_NUMBER;
      if (lhs == TYPE_STRING && rhs == TYPE_STRING) return TYPE_STRING;
      break;
    case BinaryOperatorNode::SUBTRACT_ASSIGN:
    case BinaryOperatorNode::MULTIPLY_ASSIGN:
    case BinaryOperatorNode::DIVIDE_ASSIGN:
    case BinaryOperatorNode::REMAINDER_ASSIGN:
      if (lhs == TYPE_NUMBER && rhs == TYPE_NUMBER) return TYPE_NUMBER;
      break;
    case BinaryOperatorNode::OR_ASSIGN:
    case BinaryOperatorNode::AND_ASSIGN:
      if (lhs == TYPE_BOOLEAN && rhs == TYPE_BOOLEAN) return TYPE_BOOLEAN;
      break;
    case BinaryOperatorNode::EQUAL:
    case BinaryOperatorNode::DIFFERENT:
      if ((lhs == TYPE_BOOLEAN && rhs == TYPE_BOOLEAN) ||
          (lhs == TYPE_NUMBER && rhs == TYPE_NUMBER) ||
          (lhs == TYPE_STRING && rhs == TYPE_STRING)) {
        return TYPE_BOOLEAN;
      }
      break;
    case BinaryOperatorNode::LESS:
    case BinaryOperatorNode::GREATER:
    case BinaryOperatorNode::LESS_EQUAL:
    case BinaryOperatorNode::GREATER_EQUAL:
      if ((lhs == TYPE_NUMBER && rhs == TYPE_NUMBER) ||
          (lhs == TYPE_STRING && rhs == TYPE_STRING)) {
        return TYPE_BOOLEAN;
      }
      break;
    case BinaryOperatorNode::INDEX:
      if (lhs == TYPE_STRING && rhs == TYPE_NUMBER) {
        return TYPE_STRING;
      }
      if (isTypeArray(lhs) && rhs == TYPE_NUMBER) {
        return getArrayElementType(lhs);
      }
      break;
  }
  throw SemanticError("invalid operands");
}

Value::MemoryClass SemanticAnalyzer::getMemoryClass(UnaryOperatorNode::UnaryOperator, Value::MemoryClass) {
  return Value::RVALUE;
}

Value::MemoryClass SemanticAnalyzer::getMemoryClass(BinaryOperatorNode::BinaryOperator op, Value::MemoryClass lhs,
                                                    Value::MemoryClass) {
  if (op == BinaryOperatorNode::INDEX) {
    return lhs;
  }
  if (op == BinaryOperatorNode::ASSIGN || op == BinaryOperatorNode::ADD_ASSIGN ||
      op == BinaryOperatorNode::SUBTRACT_ASSIGN || op == BinaryOperatorNode::MULTIPLY_ASSIGN ||
      op == BinaryOperatorNode::DIVIDE_ASSIGN || op == BinaryOperatorNode::REMAINDER_ASSIGN ||
      op == BinaryOperatorNode::OR_ASSIGN || op == BinaryOperatorNode::AND_ASSIGN) {
    if (lhs == Value::LVALUE) return Value::LVALUE;
    throw SemanticError("rvalue as left hand side operand of an assignment");
  }
  return Value::RVALUE;
}