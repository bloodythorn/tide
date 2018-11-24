#ifndef CLASS_HPP
#define CLASS_HPP

#include <ostream>

class Class {
public:

  /* Ctor/Xtor */
  Class();

  Class(const Class& p_ot);

  Class(Class&& p_ot);

  ~Class();

  /* Operator Overloads */
  Class& operator=(const Class& p_ot);

  Class& operator=(Class&& p_ot);

private:

  friend std::ostream& operator<<(std::ostream& p_os, const Class& p_ot) {
    p_os << typeid(p_ot).name();
    return p_os;
  }
  friend std::wostream& operator<<(std::wostream& p_os, const Class& p_ot) {
    p_os << typeid(p_ot).name();
    return p_os;
  }
};

#endif // CLASS_HPP