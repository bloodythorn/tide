#include "tide/games/checkers/checkerboard.hpp"

#include <stdexcept>

namespace tide { namespace Games { namespace Checkers {

/************************************************************/
CheckerBoard::CheckerBoard() {
  this->fill(std::make_tuple(Player::COUNT, Piece::COUNT));
}

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
void CheckerBoard::clear(Square p_sq) {
  set(p_sq, Player::COUNT, Piece::COUNT);
}

/************************************************************/
void CheckerBoard::set(Square p_sq, Player p_pl, Piece p_pi) {
  (*this)[p_sq] = std::make_tuple(p_pl, p_pi);
}

/************************************************************/
const Player CheckerBoard::getPlayer(Square p_sq) const {
  return std::get<0>((*this)[p_sq]);
}

/************************************************************/
const Piece CheckerBoard::getPiece(Square p_sq) const {
  return std::get<1>((*this)[p_sq]);
}

/************************************************************/
const bool CheckerBoard::isKing(Square p_sq) const {
  return (getPiece(p_sq) == Piece::KING);
}

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