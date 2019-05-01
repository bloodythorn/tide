#include "class.hpp"

#include <stdexcept>

Class::Class() { throw std::runtime_error("Unimplemented Constructor"); }

Class::Class(const Class& p_ot) {
  throw std::runtime_error("Unimplemented Copy Constructor");
}

Class::Class(Class&& p_ot) {
  throw std::runtime_error("Unimplemented move Constructor");
}

Class::~Class() { }

Class& Class::operator=(const Class& p_ot) {
  throw std::runtime_error("Unimplemented Copy Assignment");
  if(this != &p_ot) {};
  return *this;
}

Class& Class::operator=(Class&& p_ot) {
  throw std::runtime_error("Unimplemented Move Assignment");
  if(this != &p_ot) {};
  return *this;
}

