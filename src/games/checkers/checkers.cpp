#include "tide/games/checkers/checkers.hpp"

#include <tide/engine/logger.hpp>

namespace tide { namespace Games { namespace Checkers {

#include <stdexcept>

const std::wstring Checkers::MODULE_NAME{L"Checkers"};

Checkers::Checkers() { }

Checkers::Checkers(const Checkers& p_ot) {
  throw std::runtime_error("Unimplemented Copy Constructor");
}

Checkers::Checkers(Checkers&& p_ot) {
  throw std::runtime_error("Unimplemented move Constructor");
}

Checkers::~Checkers() { }

std::string Checkers::listMoves(void) { return "Implement me!"; }

std::string Checkers::rlistMoves(void) { return "Implement me!"; }

bool Checkers::newGame(void) { return load(ENGLISH_DR); }

bool Checkers::load(const std::string& p_game) {

  std::stringstream ss{p_game};

  /* get player */
  char pl; ss >> pl;
  if((m_pl = charToTurn(pl)) == Player::ERROR) {
    LOG_ERROR(MODULE_NAME) << Engine::Log::ff(__func__)
      << "Incorrect Player Number " << pl;
    return false;
  }

  /* Place pieces */
  char c;
  int8_t index;
  while(ss >> c) {
    m_cb.set(index, charToPlayer(c), charToPiece(c));
    if((m_cb.getPiece(index) == Piece::ERROR) ||
        m_cb.getPlayer(index) == Player::ERROR) {
      LOG_FATAL(MODULE_NAME) << Engine::Log::ff(__func__)
        << "Unrecognized character: " << c;
      return false;
    }

    /* Over max count? */
    if(index++ > SQUARE_COUNT-1) {
      LOG_FATAL(MODULE_NAME) << Engine::Log::ff(__func__)
        << "Game load string longer than allowed: " << SQUARE_COUNT;
    return false;
    }
  }

  /* Under max count? */
  if(index++ < SQUARE_COUNT-1) {
      LOG_FATAL(MODULE_NAME) << Engine::Log::ff(__func__)
        << "Game load string shorter than allowed: " << SQUARE_COUNT;
    return false;
  }

  return true;
}

std::string Checkers::save(void) { return ""; }

bool Checkers::move(unsigned char, unsigned char) { return false; }

bool Checkers::rmove(unsigned char, unsigned char) { return false; }

const Player Checkers::charToPlayer(const char& p_c) const {
  switch(p_c) {
    case 'b': case 'B': return Player::BLACK;
    case 'r': case 'R': return Player::RED;
    case 'x': return Player::COUNT;
    default: return Player::ERROR;
  }
}

const Player Checkers::charToTurn(const char& p_c) const {
  switch(p_c) {
    case '0': return Player::BLACK;
    case '1': return Player::RED;
    default: return Player::ERROR;
  }
}

const Piece Checkers::charToPiece(const char& p_c) const {
  switch(p_c) {
    case 'b': case 'r': return Piece::MAN;
    case 'B': case 'R': return Piece::KING;
    case 'x': return Piece::COUNT;
    default: return Piece::ERROR;
  }
}

const char Checkers::PosToChar(const CheckerBoard&, const Square&) const {
  return ' ';
}

Checkers& Checkers::operator=(const Checkers& p_ot) {
  throw std::runtime_error("Unimplemented Copy Assignment");
  if(this != &p_ot) {};
  return *this;
}

Checkers& Checkers::operator=(Checkers&& p_ot) {
  throw std::runtime_error("Unimplemented Move Assignment");
  if(this != &p_ot) {};
  return *this;
}

}/*Checkers*/}/*Games*/}/*tide*/