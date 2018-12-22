#include "tide/engine/platform.hpp"

namespace tide { namespace Engine { namespace Platform {

Platform::Platform() { }

Platform::Platform(const Platform& p_ot) { }

Platform::Platform(Platform&& p_ot) { }

Platform::~Platform() { }

Platform& Platform::operator=(const Platform& p_ot) {
  if(this != &p_ot) {};
  return *this;
}

Platform& Platform::operator=(Platform&& p_ot) {
  if(this != &p_ot) {};
  return *this;
}

}/*Platform*/}/*Engine*/}/*tide*/