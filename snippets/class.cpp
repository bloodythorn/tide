#include "class.hpp"

Class::Class() { }

Class::Class(const Class& p_ot) { }

Class::Class(Class&& p_ot) { }

Class::~Class() { }

Class& Class::operator=(const Class& p_ot) {
  if(this != &p_ot) {};
  return *this;
}

Class& Class::operator=(Class&& p_ot) {
  if(this != &p_ot) {};
  return *this;
}

