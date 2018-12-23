#include <cstdlib>
#include <exception>
#include <iostream>

#include <tide/engine/engine.hpp>
#include <tide/engine/logger.hpp>

class State_Test : public tide::Engine::State {
public:
  const std::wstring Name(void) const override { return L"Test"; }
  void update(tide::Engine::Engine& p_en, double p_dt) override {}
  void render(tide::Engine::Engine& p_en, double p_dt) override {}
};

class CLITest : public tide::Engine::Engine {
public:
  void Start(void) override {
    m_sm.addStartState(
    tide::Engine::StateManager::StatePtr(new State_Test()));
    start();
  }

};

int main(int argc, char** argv) {

  try {
    CLITest t;
    t.Start();
  }
  catch (std::exception& e) {
    LOG_DEBUG(L"test_p_CLI") << L":Exception->" << e.what();
  }

  exit(EXIT_SUCCESS);
}