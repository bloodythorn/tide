#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

#include <tide/engine/engine.hpp>
#include <tide/engine/logger.hpp>
#include <tide/engine/plat_cli.hpp>

const std::wstring MODULE_NAME{L"test_p_CLI"};

/************************************************************/
class State_Test : public tide::Engine::State {
public:
  ~State_Test(void) {}

  const std::wstring Name(void) const override { return L"Test"; }
  void handle(tide::Engine::Engine& p_en, double p_dt) override {
    uint8_t count{};
    if(!p_en.hasSysEvent())
      LOG_DEBUG(MODULE_NAME) << __func__ << ":No System Events!";
    while(count < 5 && p_en.hasSysEvent()) {
      p_en.getSysEvent();
      ++count;
    }
  }
  void update(tide::Engine::Engine& p_en, double p_dt) override {}
  void render(tide::Engine::Engine& p_en, double p_dt) override {}
};

/************************************************************/
class CLITest : public tide::Engine::Engine {
public:

  CLITest(void) {
    /* Add Start/Only state */
    m_sm.addStartState(
    tide::Engine::StateManager::StatePtr(new State_Test()));
  }

  void Start(void) override { start(); }

};

/************************************************************/
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