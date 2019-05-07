#ifndef CHECKERS_HPP
#define CHECKERS_HPP

#include <ostream>
#include <string>

#include "tide/engine/logger.hpp"
#include "tide/games/checkers/checkerboard.hpp"

namespace tide { namespace Games { namespace Checkers {

static const std::string ENGLISH_DR{"0bbbbbbbbbbbbxxxxxxxxrrrrrrrrrrrr"};

class Checkers {
public:

  static const std::wstring MODULE_NAME;

  /* Ctor/Xtor */
  Checkers(void);

  Checkers(const Checkers& p_ot);

  Checkers(Checkers&& p_ot);

  ~Checkers();

  /* Member Functions */
  std::string save(void);
  std::string listMoves(void);
  std::string rlistMoves(void);
  bool newGame(void);
  bool load(const std::string&);
  bool move(Square, Square);
  bool rmove(Square, Square);
  bool unMove(Square, Square);

  /* Operator Overloads */
  Checkers& operator=(const Checkers& p_ot);

  Checkers& operator=(Checkers&& p_ot);

private:

  /* Data */
  CheckerBoard m_cb;
  Player       m_pl;

  friend std::ostream& operator<<(std::ostream& p_os, const Checkers& p_ot) {
    p_os << typeid(p_ot).name();
    return p_os;
  }
  friend std::wostream& operator<<(std::wostream& p_os, const Checkers& p_ot) {
    p_os << typeid(p_ot).name();
    return p_os;
  }

};

}/*Checkers*/}/*Games*/}/*tide*/

#endif //CHECKERS_HPP