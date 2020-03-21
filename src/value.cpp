#include "value.h"

#include "store.h"

int Lvalue::getType() const { return getRvalue()->getType(); }

const Rvalue* Lvalue::getRvalue() const { return store.getValue(name).get(); }

void Lvalue::setValue(std::unique_ptr<Rvalue> value) const { store.setValue(name, std::move(value)); }

const Rvalue* ElementLvalue::getRvalue() const {
  auto rv = store.getValue(name).get();
  for (int i : index) {
    rv = dynamic_cast<ArrayRvalue*>(rv)->getValue()->at(i).get();
  }
  return rv;
}

void ElementLvalue::setValue(std::unique_ptr<Rvalue> value) const {
  store.setValue(name, index, std::move(value));
}
