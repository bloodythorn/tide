#ifndef STATE_CONSOLE_HPP
#define STATE_CONSOLE_HPP

#include <tide/engine/state.hpp>

class State_Console : public tide::Engine::State {
public:
  const std::wstring Name(void) const override;
protected:
private:
};

#endif //STATE_CONSOLE_HPP