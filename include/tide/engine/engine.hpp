#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <memory> //unique_ptr

#include "tide/engine/logger.hpp"
#include "tide/engine/loop.hpp"
#include "tide/engine/noncopyable.hpp"
#include "tide/engine/platform.hpp"
#include "tide/engine/state.hpp"
#include "tide/engine/statemanager.hpp"

namespace tide { namespace Engine {

class Engine : protected Loop, private NonCopyable {
public:

  Engine(void);
  virtual void Start(void) = 0;
  bool hasSysEvent(void);
  Platform::SysEventPtr getSysEvent(void) const;

protected:

  StateManager m_sm;
  std::unique_ptr<Platform::Platform> m_platform;

private:

  void handle(double) override;
  void update(double) override;
  void render(double) override;
};
}/*Engine*/}/*tide*/

#endif //ENGINE_HPP