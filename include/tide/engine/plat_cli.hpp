#ifndef PLAT_CLI_HPP
#define PLAT_CLI_HPP

#include "tide/engine/platform.hpp"

#include <ostream>

namespace tide { namespace Engine { namespace Platform {

class Plat_CLI : public Platform {
public:

  /* Ctor/Xtor */
  Plat_CLI();

  Plat_CLI(const Plat_CLI& p_ot);

  Plat_CLI(Plat_CLI&& p_ot);

  ~Plat_CLI();

  /* Operator Overloads */
  Plat_CLI& operator=(const Plat_CLI& p_ot);

  Plat_CLI& operator=(Plat_CLI&& p_ot);

private:

  friend std::ostream& operator<<(std::ostream& p_os, const Plat_CLI& p_ot) {
    p_os << typeid(p_ot).name();
    return p_os;
  }
  friend std::wostream& operator<<(std::wostream& p_os, const Plat_CLI& p_ot) {
    p_os << typeid(p_ot).name();
    return p_os;
  }
};

}/*Platform*/}/*Engine*/}/*tide*/

#endif // PLAT_CLI_HPP
