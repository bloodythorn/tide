#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <ostream>
#include <queue>
#include <string>

#include "tide/engine/logger.hpp"

namespace tide { namespace Engine { namespace Options {

/** @brief Option Class */
using Args = std::vector<std::string>;
struct Option {
  std::string name;
  Args args;
};

/* @brief Module Name */
static const std::wstring MODULE_NAME{L"Option"};

/** @class ArgParser
 *
 * @ingroup Engine
 *
 * This class, given command line arguments will parse them into groups of
 * commands (beginning with a '-'), and arguments.
 *
 */
class ArgParser : public std::queue<Option> {
public:

  /* Ctor/Xtor */
  ArgParser(int, char**);

  ArgParser(const ArgParser& p_ot);

  ArgParser(ArgParser&& p_ot);

  ~ArgParser();

  /* Operator Overloads */
  ArgParser& operator=(const ArgParser& p_ot);

  ArgParser& operator=(ArgParser&& p_ot);

private:

  /* Member Variables */
  std::string m_exeName{""};

  friend std::ostream& operator<<(std::ostream& p_os, const ArgParser& p_ot) {
    p_os << typeid(p_ot).name();
    return p_os;
  }
  friend std::wostream& operator<<(std::wostream& p_os, const ArgParser& p_ot) {
    p_os << typeid(p_ot).name();
    return p_os;
  }
};

}/*Options*/}/*Engine*/ }/*tide*/

#endif // OPTIONS_HPP