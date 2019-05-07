#include "tide/engine/options.hpp"

#include <stdexcept>

#include <deque>
#include <vector>

namespace tide { namespace Engine { namespace Options {

/************************************************************/
ArgParser::ArgParser(int p_argc, char** p_argv) {

  /* Put args in deque */
  std::deque<std::string> exeArgs;
  for(auto i = 0; i < p_argc; ++i) exeArgs.push_back(p_argv[i]);

  /* Pull out executable name */
  m_exeName = exeArgs.front(); exeArgs.pop_front();

  /* Extract each option and it's args */
  while(!exeArgs.empty()) {
    /* Verify it's a valid command */
    if(!exeArgs.empty() && exeArgs.front()[0] == '-') {
      Option option;
      option.name = exeArgs.front(); exeArgs.pop_front();

      /* Parse Arguments to command */
      while(!exeArgs.empty() && exeArgs.front()[0] != '-'){
        option.args.push_back(exeArgs.front());
        exeArgs.pop_front();
      }

      /* Save it */
      this->push(option);

    } else
      throw
        std::runtime_error(
          "Error parsing command; wrong order in" + exeArgs.front()
        );
  }
}

/************************************************************/
ArgParser::ArgParser(const ArgParser& p_ot) {
  throw std::runtime_error("Unimplemented Copy Constructor");
}

ArgParser::ArgParser(ArgParser&& p_ot) {
  throw std::runtime_error("Unimplemented move Constructor");
}

/************************************************************/
ArgParser::~ArgParser() { }

/************************************************************/
ArgParser& ArgParser::operator=(const ArgParser& p_ot) {
  throw std::runtime_error("Unimplemented Copy Assignment");
  if(this != &p_ot) {};
  return *this;
}

/************************************************************/
ArgParser& ArgParser::operator=(ArgParser&& p_ot) {
  throw std::runtime_error("Unimplemented Move Assignment");
  if(this != &p_ot) {};
  return *this;
}

}/*Options*/}/*Engine*/ }/*tide*/