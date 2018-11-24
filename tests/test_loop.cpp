#include <cstdlib>
#include <iostream>
#include <string>
#include <functional>

#include <tide/engine/engine.hpp>

static const std::wstring MODULE_NAME{L"LoopTest"};

class LoopTest : private tide::Engine::Loop {
private:

  void handle(double dt) override {
    LOG_INFO(MODULE_NAME) << __func__ << "|Delta:" << dt;
  }
  void update(double dt) override {
    LOG_INFO(MODULE_NAME) << __func__ << "|Delta:" << dt;
    if(getFrame() > 10) stop();
  }
  void render(double dt) override {
    LOG_INFO(MODULE_NAME)
      << __func__ << ":"
      << "AFps:" << getAvgFPS()
      << "|Delta:" << dt;
  }

public:

  LoopTest(int p_argc, char** p_argv) : Loop(60) { };

  void Run(void) { start(); }
};

int main(int argc, char** argv) {
  LOG_INFO(MODULE_NAME) << __func__ << ": Starting Tests";

  LoopTest test(argc, argv);
  test.Run();

  return EXIT_SUCCESS;
}
