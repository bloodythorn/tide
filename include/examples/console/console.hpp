#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <string>

#include <tide/engine/engine.hpp>

class Console : protected tide::Engine::Engine {
public:

  Console(int, char**);

  void Start(void);

protected:

private:

};

#endif

