#ifndef BEAUTY_LANG_VM_H
#define BEAUTY_LANG_VM_H

#include "node.h"
#include "value.h"

class VirtualMachine {
  public:
    static void run(Node* node);

  private:
    static Value* evalExp(ExpressionNode* node);
};

#endif //BEAUTY_LANG_VM_H
