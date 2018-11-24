#include "tide/engine/engine.hpp"

namespace tide {
namespace Engine {

/*TODO:FIXME1 */
Engine::Engine(void) : Loop(60) { }

void Engine::handle(double p_dt) { m_sm.handle(*this, p_dt); }
void Engine::update(double p_dt) { m_sm.update(*this, p_dt); }
void Engine::render(double p_dt) { m_sm.render(*this, p_dt); }

}//Engine
}//tide