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
  int pl;
  ss >> pl;
  switch(pl) {
    case 0 : { m_pl = Player::BLACK; break; }
    case 1 : { m_pl = Player::RED; break; }
    default : {
      LOG_ERROR(MODULE_NAME) << Engine::Log::ff(__func__)
        << "Incorrect Player Number " << pl;
      break;
    }
  }

  /* Place pieces */
  char c;
  int8_t index;
  while(ss >> c) {

    switch(c) {
      case 'x': { m_cb.clear(index); break; }
      case 'b': { m_cb.set(index, Player::BLACK, Piece::MAN); break; }
      case 'B': { m_cb.set(index, Player::BLACK, Piece::KING); break; }
      case 'r': { m_cb.set(index, Player::RED, Piece::MAN); break; }
      case 'R': { m_cb.set(index, Player::RED, Piece::KING); break; }
      default: {
        LOG_FATAL(MODULE_NAME) << Engine::Log::ff(__func__)
          << "Unrecognized character: " << c;
        return false;
      }
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