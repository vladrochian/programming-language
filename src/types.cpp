#include "types.h"

namespace {
const int BASE = 7;
}

int TYPE_ARRAY(int type) {
  return BASE * type + 4;
}

bool isTypeArray(int type) {
  return type % BASE == 4;
}

int getArrayElementType(int arrayType) {
  return arrayType / BASE;
}

int TYPE_OBJ(int temp) {
  return BASE * temp + 5;
}

bool isTypeObj(int type) {
  return type % BASE == 5;
}

int getObjTemplate(int objType) {
  return objType / BASE;
}

int TYPE_LIST(int type) {
  return BASE * type + 6;
}

bool isTypeList(int type) {
  return type % BASE == 6;
}

int getListElementType(int listType) {
  return listType / BASE;
}

