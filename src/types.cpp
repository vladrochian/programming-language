#include "types.h"

namespace {
const int BASE = 8;
}

int TYPE_ARRAY(int type) {
  return BASE * type + 5;
}

bool isTypeArray(int type) {
  return type % BASE == 5;
}

int getArrayElementType(int arrayType) {
  return arrayType / BASE;
}

int TYPE_OBJ(int temp) {
  return BASE * temp + 6;
}

bool isTypeObj(int type) {
  return type % BASE == 6;
}

int getObjTemplate(int objType) {
  return objType / BASE;
}

int TYPE_LIST(int type) {
  return BASE * type + 7;
}

bool isTypeList(int type) {
  return type % BASE == 7;
}

int getListElementType(int listType) {
  return listType / BASE;
}
