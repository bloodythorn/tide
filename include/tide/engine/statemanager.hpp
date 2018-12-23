#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include <ostream>
#include <memory>
#include <string>
#include <unordered_map>

#include <tide/engine/state.hpp>

namespace tide {
namespace Engine {
  class Engine;

class StateManager {
public:

  /* Data Type Aliases */
  using StatePtr = std::shared_ptr<State>;

  /* Ctor/Xtor */
  StateManager();

  StateManager(const StateManager& p_ot);

  StateManager(StateManager&& p_ot);

  ~StateManager();

  /* Operator Overloads */
  StateManager& operator=(const StateManager& p_ot);

  StateManager& operator=(StateManager&& p_ot);

  /* Member Functions */
  void addStartState(StatePtr p_s);

  void handle(Engine&, double);
  void update(Engine&, double);
  void render(Engine&, double);

private:

  /* Data Type Aliases */
  using StateMap =
    std::unordered_map<std::wstring, StatePtr>;

  /* Data Members */

  StateMap     m_states;
  std::wstring m_startState{L""};
  std::wstring m_currentState{L""};

  friend std::ostream& operator<<(std::ostream& p_os, const StateManager& p_ot) {
    p_os << typeid(p_ot).name();
    return p_os;
  }
  friend std::wostream& operator<<(std::wostream& p_os, const StateManager& p_ot) {
    p_os << typeid(p_ot).name();
    return p_os;
  }
};
}
}

#endif // STATEMANAGER_HPP