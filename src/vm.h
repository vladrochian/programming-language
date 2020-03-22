#ifndef PROG_LANG_VM_H
#define PROG_LANG_VM_H

#include "node.h"
#include "value.h"

class VirtualMachine {
 public:
  static std::pair<bool, std::unique_ptr<Value>> run(Node* node);

 private:
  static std::unique_ptr<Value> evalExp(ExpressionNode* node);
  static bool getBooleanValue(const std::unique_ptr<Value>& value);
  static double getNumberValue(const std::unique_ptr<Value>& value);
  static std::string getStringValue(const std::unique_ptr<Value>& value);
};

#endif //PROG_LANG_VM_H
