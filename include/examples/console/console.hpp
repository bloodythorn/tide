#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <string>

#include <tide/engine/engine.hpp>

class Console : protected tide::Engine::Engine {
public:

  static const std::wstring MODULE_NAME;

  Console(int, char**);

  void Start(void);

protected:

private:

};

#endif

