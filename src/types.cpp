#include "types.h"

namespace {
const int BASE = 6;
}

int TYPE_ARRAY(int type) {
  return BASE * type + 4;
};

int TYPE_OBJ(int temp) {
  return BASE * temp + 5;
}
