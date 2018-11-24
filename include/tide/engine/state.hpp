#ifndef STATE_HPP
#define STATE_HPP

#include <string>

namespace tide {
namespace Engine {

class State {
public:
  virtual const std::wstring Name(void) const = 0;
protected:
private:
};
}
}

#endif //STATE_HPP