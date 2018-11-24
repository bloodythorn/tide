#include "tide/engine/statemanager.hpp"

namespace tide { namespace Engine {

StateManager::StateManager() { }

StateManager::StateManager(const StateManager& p_ot) {
  typeid(p_ot).name();
}

StateManager::StateManager(StateManager&& p_ot) {
  typeid(p_ot).name();
}

StateManager::~StateManager() { }

StateManager& StateManager::operator=(const StateManager& p_ot) {
  typeid(p_ot).name();
  return *this;
}

StateManager& StateManager::operator=(StateManager&& p_ot) {
  typeid(p_ot).name();
  if(this == &p_ot) return *this;
  return *this;
}

void StateManager::addStartState(StateManager::StatePtr p_st){
  if(m_states.find(p_st->Name()) != m_states.end()) {/*throw*/}
  m_startState = p_st->Name();
}

void StateManager::handle(Engine& p_en, double p_dt) {
  auto st = m_states.find(m_currentState);
  if(st == m_states.end()) {/*throw*/}
}

void StateManager::update(Engine& p_en, double p_dt) {
  auto st = m_states.find(m_currentState);
  if(st == m_states.end()) {/*throw*/}
}

void StateManager::render(Engine& p_en, double p_dt) {
  auto st = m_states.find(m_currentState);
  if(st == m_states.end()) {/*throw*/}
}

}/*engine*/}/*tide*/