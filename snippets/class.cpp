#include "class.hpp"

Class::Class() { }

Class::Class(const Class& p_ot) {
  typeid(p_ot).name();
}

Class::Class(Class&& p_ot) {
  typeid(p_ot).name();
}

Class::~Class() { }

Class& Class::operator=(const Class& p_ot) {
  typeid(p_ot).name();
  return *this;
}

Class& Class::operator=(Class&& p_ot) {
  typeid(p_ot).name();
  if(this == &p_ot) return *this;
  return *this;
}

