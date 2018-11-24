#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "tide/engine/logger.hpp"
#include "tide/engine/loop.hpp"
#include "tide/engine/noncopyable.hpp"
#include "tide/engine/state.hpp"
#include "tide/engine/statemanager.hpp"

namespace tide {
namespace Engine {

class Engine : protected Loop, private NonCopyable {
public:
//  Engine(void);
//protected:
//
//  StateManager m_sm;
//
//private:
//
//  void handle(double) override;
//  void update(double) override;
//  void render(double) override;
};
}
}

#endif //ENGINE_HPP