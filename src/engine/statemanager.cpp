#include "tide/engine/statemanager.hpp"

#include <stdexcept>

namespace tide { namespace Engine {

StateManager::StateManager() { }

StateManager::StateManager(const StateManager& p_ot)
: m_startState(p_ot.m_startState), m_currentState(p_ot.m_currentState) {
  m_states.clear();
  for(const auto& a : p_ot.m_states) {
    m_states.insert({a.first, StatePtr(a.second)});
  }
}

StateManager::StateManager(StateManager&& p_ot)
: m_startState(p_ot.m_startState), m_currentState(p_ot.m_currentState) {
  m_states.clear();
  for(const auto& a : p_ot.m_states) {
    m_states.insert({a.first, StatePtr(a.second)});
  }
  p_ot.m_startState = L"";
  p_ot.m_currentState = L"";
  p_ot.m_states.clear();
}

StateManager::~StateManager() { }

StateManager& StateManager::operator=(const StateManager& p_ot) {
  if(this == &p_ot) {
    m_startState = p_ot.m_startState;
    m_currentState = p_ot.m_currentState;
    m_states.clear();
    for(const auto& a : p_ot.m_states) {
      m_states.insert({a.first, StatePtr(a.second)});
    }
  }
  return *this;
}

StateManager& StateManager::operator=(StateManager&& p_ot) {
  if(this == &p_ot) {
    m_startState = p_ot.m_startState;
    p_ot.m_startState = L"";
    m_currentState = p_ot.m_currentState;
    p_ot.m_currentState = L"";
    m_states.clear();
    for(const auto& a : p_ot.m_states) {
      m_states.insert({a.first, StatePtr(a.second)});
    }
    p_ot.m_states.clear();
  }
  return *this;
}

void StateManager::addStartState(StateManager::StatePtr p_st){
  if(m_states.find(p_st->Name()) != m_states.end())
    throw std::runtime_error("State already exists!");
  if(m_states.size() == 0) m_startState = p_st->Name();
  m_states.insert({p_st->Name(), p_st});
  m_currentState = p_st->Name();
}

void StateManager::handle(Engine& p_en, double p_dt) {
  auto st = m_states.find(m_currentState);
  if(st == m_states.end())
    throw std::out_of_range("Current state not found!");
  //HANDLE ACTIONS
}

void StateManager::update(Engine& p_en, double p_dt) {
  auto st = m_states.find(m_currentState);
  if(st == m_states.end())
    throw std::out_of_range("Current state not found!");
  //st->second->update(p_en, p_dt);
}

void StateManager::render(Engine& p_en, double p_dt) {
  auto st = m_states.find(m_currentState);
  if(st == m_states.end())
    throw std::out_of_range("Current state not found!");
  //st->second->render(p_en, p_dt);
}

}/*engine*/}/*tide*/