#include "tide/engine/plat_cli.hpp"

#include <stdexcept>

namespace tide { namespace Engine { namespace Platform {

Plat_CLI::Plat_CLI() { throw new std::runtime_error("Unimplemented Constructor"); }

Plat_CLI::Plat_CLI(const Plat_CLI& p_ot) {
  throw new std::runtime_error("Unimplemented Copy Constructor");
}

Plat_CLI::Plat_CLI(Plat_CLI&& p_ot) {
  throw new std::runtime_error("Unimplemented move Constructor");
}

Plat_CLI::~Plat_CLI() { }

Plat_CLI& Plat_CLI::operator=(const Plat_CLI& p_ot) {
  throw new std::runtime_error("Unimplemented Copy Assignment");
  if(this != &p_ot) {}
  return *this;
}

Plat_CLI& Plat_CLI::operator=(Plat_CLI&& p_ot) {
  throw new std::runtime_error("Unimplemented Move Assignment");
  if(this != &p_ot) {}
  return *this;
}

}/*Platform*/}/*Engine*/}/*tide*/