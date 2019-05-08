#include "tide/engine/engine.hpp"

#include "tide/engine/logger.hpp"

namespace tide {
namespace Engine {

/*TODO:FIXME1 */
Engine::Engine(void) : Loop(60) { }

bool Engine::hasSysEvent(void) {
  if(!m_platform) {
    LOG_DEBUG(MODULE_NAME) << __func__ << L": No Platform.";
    return false;
  }
  /*TODO: Finsish this function */
  return false;
}

Platform::SysEventPtr Engine::getSysEvent(void) const {
  /*TODO: Finsish this function */
  return Platform::SysEventPtr();
}

void Engine::handle(double p_dt) {
  LOG_DEBUG(MODULE_NAME) << __func__ << ":Handle" << p_dt;
  m_sm.handle(*this, p_dt);
}
void Engine::update(double p_dt) {
  LOG_DEBUG(MODULE_NAME) << __func__ << ":Update" << p_dt;
  m_sm.update(*this, p_dt);
}
void Engine::render(double p_dt) {
  LOG_DEBUG(MODULE_NAME) << __func__ << ":Render" << p_dt;
  m_sm.render(*this, p_dt);
}

}//Engine
}//tide