#ifndef STATE_CONSOLE_HPP
#define STATE_CONSOLE_HPP

#include <tide/engine/engine.hpp>
#include <tide/engine/state.hpp>

class State_Console : public tide::Engine::State {
public:
  const std::wstring Name(void) const override;
  void handle(tide::Engine::Engine& p_en, double p_dt) override;
  void update(tide::Engine::Engine& p_en, double p_dt) override;
  void render(tide::Engine::Engine& p_en, double p_dt) override;
protected:
private:
};

#endif //STATE_CONSOLE_HPP