#include "tide/games/checkers/checkerboard.hpp"

#include <stdexcept>

namespace tide { namespace Games { namespace Checkers {

/************************************************************/
CheckerBoard::CheckerBoard() { board.fill(Piece::COUNT);  }

/************************************************************/
CheckerBoard::CheckerBoard(const CheckerBoard& p_ot) {
  throw std::runtime_error("Unimplemented Copy Constructor");
}

/************************************************************/
CheckerBoard::CheckerBoard(CheckerBoard&& p_ot) {
  throw std::runtime_error("Unimplemented move Constructor");
}

/************************************************************/
CheckerBoard::~CheckerBoard() { }

/************************************************************/
void CheckerBoard::clear(Square) { }

/************************************************************/
void CheckerBoard::set(Square, Player, Piece) {}

/************************************************************/
CheckerBoard& CheckerBoard::operator=(const CheckerBoard& p_ot) {
  throw std::runtime_error("Unimplemented Copy Assignment");
  if(this != &p_ot) {};
  return *this;
}

/************************************************************/
CheckerBoard& CheckerBoard::operator=(CheckerBoard&& p_ot) {
  throw std::runtime_error("Unimplemented Move Assignment");
  if(this != &p_ot) {};
  return *this;
}

}/*Checkers*/}/*Games*/}/*tide*/