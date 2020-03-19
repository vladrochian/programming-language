#include "value.h"

#include "store.h"

int Lvalue::getType() const { return getRvalue()->getType(); }

const Rvalue* Lvalue::getRvalue() const { return store.getValue(name).get(); }

void Lvalue::setValue(std::unique_ptr<Rvalue> value) const { store.setValue(name, std::move(value)); }
