#ifndef STATE_HPP
#define STATE_HPP

#include <string>

namespace tide {
namespace Engine {

class Engine;

class State {
public:
  virtual const std::wstring Name(void) const = 0;
  virtual void handle(Engine& p_en, double p_dt) = 0;
  virtual void update(Engine& p_en, double p_dt) = 0;
  virtual void render(Engine& p_en, double p_dt) = 0;
protected:
private:
};
}
}

#endif //STATE_HPP