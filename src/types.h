#ifndef PROG_LANG_TYPES_H
#define PROG_LANG_TYPES_H

const int TYPE_NONE = 0;
const int TYPE_BOOLEAN = 1;
const int TYPE_NUMBER = 2;
const int TYPE_STRING = 3;
const int TYPE_MIXED = 4;

int TYPE_ARRAY(int type);
bool isTypeArray(int type);
int getArrayElementType(int arrayType);

int TYPE_OBJ(int temp);
bool isTypeObj(int type);
int getObjTemplate(int objType);

int TYPE_LIST(int type);
bool isTypeList(int type);
int getListElementType(int listType);

#endif //PROG_LANG_TYPES_H
