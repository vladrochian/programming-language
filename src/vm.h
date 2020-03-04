#ifndef PROG_LANG_VM_H
#define PROG_LANG_VM_H

#include "node.h"
#include "value.h"

class VirtualMachine {
  public:
    static void run(Node* node);

  private:
    static std::unique_ptr<Value> evalExp(ExpressionNode* node);
};

#endif //PROG_LANG_VM_H
