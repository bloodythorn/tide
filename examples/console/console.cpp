#include "examples/console/console.hpp"

#include "examples/console/state_console.hpp"

#include <tide/engine/logger.hpp>

#include <memory>

Console::Console(int, char**) { }

void Console::Start(void) {
  m_sm.addStartState(
    tide::Engine::StateManager::StatePtr(new State_Console()));
  start();
}

/** Main Entry Point **/
#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
  static const std::wstring MODULE_NAME{L"Main"};
  LOG_INFO(MODULE_NAME) << __func__ << "()-Initializing Logger.";
  LOG_STAT.setReportLevel(MODULE_NAME, tide::Engine::Log::Debug);

  LOG_INFO(MODULE_NAME) << __func__ << "()-Starting Engine.";
  try { Console c(argc, argv); c.Start(); }
  catch(const std::exception& e) {
    LOG_ERROR(MODULE_NAME) << __func__ << ":Exception:" << e.what();
    return EXIT_FAILURE;
  }
  catch(...) {
    LOG_ERROR(MODULE_NAME) << __func__ << ":Unknown Exception";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
