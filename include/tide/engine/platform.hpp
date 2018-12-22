#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <ostream>

namespace tide { namespace Engine { namespace Platform {

class Platform {
public:

  /* Ctor/Xtor */
  Platform();

  Platform(const Platform& p_ot);

  Platform(Platform&& p_ot);

  ~Platform();

  /* Operator Overloads */
  Platform& operator=(const Platform& p_ot);

  Platform& operator=(Platform&& p_ot);

private:

  friend std::ostream& operator<<(std::ostream& p_os, const Platform& p_ot) {
    p_os << typeid(p_ot).name();
    return p_os;
  }
  friend std::wostream& operator<<(std::wostream& p_os, const Platform& p_ot) {
    p_os << typeid(p_ot).name();
    return p_os;
  }
};
}/*Platform*/}/*Engine*/}/*tide*/

#endif // PLATFORM_HPP
