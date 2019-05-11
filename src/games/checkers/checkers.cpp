#include "tide/games/checkers/checkers.hpp"

#include <tide/engine/logger.hpp>

#include <algorithm>
#include <cmath>
#include <iterator>
#include <vector>

namespace tide { namespace Games { namespace Checkers {

#include <stdexcept>

const std::wstring Checkers::MODULE_NAME{L"Checkers"};

/*************************************************************/
Checkers::Checkers() { }

/*************************************************************/
Checkers::Checkers(const Checkers& p_ot) { }

/*************************************************************/
Checkers::Checkers(Checkers&& p_ot) { }

/*************************************************************/
Checkers::~Checkers() { }

bool Checkers::newGame(void) { return load(ENGLISH_DR); }

/*************************************************************/
bool Checkers::load(const std::string& p_game) {

  /* Check size to make sure input is 33 char */
  if(p_game.size() != SQUARE_COUNT+1) {
    LOG_FATAL(MODULE_NAME) << Engine::Log::ff(__func__)
      << "Input string is incorrect size. Must be "
      << SQUARE_COUNT+1;
    return false;
  }

  /* get player */
  if((m_pl = charToTurn(p_game[0])) == Player::ERROR) {
    LOG_ERROR(MODULE_NAME) << Engine::Log::ff(__func__)
      << "Incorrect Player Number " << p_game[0];
    return false;
  }

  /* Place pieces */
  size_t index{0};
  auto iter = ++p_game.cbegin();
  while(index < SQUARE_COUNT) {

    m_cb.set(
      index,
      charToPlayer(*iter),
      charToPiece(*iter));

    if(checkError(m_cb[index])) {
      LOG_FATAL(MODULE_NAME) << Engine::Log::ff(__func__)
        << "Unrecognized character: " << *iter;
      return false;
    }

    ++index; ++iter;
  }

  return true;
}

std::string Checkers::save(void) { return ""; }

const std::string Checkers::listMoves(void) const{
  /* Helper Functions */
  auto isPlayer =
    [&](const Contents& c) ->bool { return std::get<0>(c) == m_pl; };

  /* Output */
  std::stringstream ss;

  /* Find each piece that is owned by the current player */
  //auto iter = std::find_if(m_cb.begin(), m_cb.end(), isPlayer);
  auto iter = m_cb.cbegin();
  while(iter != m_cb.end()) {
    auto pos = std::distance(m_cb.begin(), iter);

    LOG_DEBUG(MODULE_NAME) << Engine::Log::ff(__func__)
      << "Pos: " << pos
      << " Row: " << static_cast<int>(pos / 4) << " Col: " << (pos % 4);

    auto pl = std::get<0>(*iter);
    auto BLKMoves = getDiags(pos, Player::BLACK);
    auto REDMoves = getDiags(pos, Player::RED);

    for(const auto& a : BLKMoves)
      LOG_DEBUG(MODULE_NAME) << Engine::Log::ff(__func__) << a;
    for(const auto& a : REDMoves)
      LOG_DEBUG(MODULE_NAME) << Engine::Log::ff(__func__) << a;

    //iter = std::find_if(++iter, m_cb.end(), isPlayer);;
    ++iter;
  }

  return ss.str();
}

const std::string Checkers::rlistMoves(void) const { return "Implement me!"; }

const std::vector<Square> Checkers::getDiags(
  const Square& p_s,
  const Player& p_p) const {
    std::vector<Square> out;

    auto row = static_cast<int>(p_s / COLS);
    if((p_p == Player::BLACK) && (row != 7)) {
      out.push_back(p_s + COLS);
      if((row % 2 == 0) && (p_s % COLS != 3)) {
        out.push_back(p_s + COLS + 1);
      } else
      if((row % 2 == 1) && (p_s % COLS != 0)) {
        out.push_back(p_s + COLS - 1);
      }
    } else
    if ((p_p == Player::RED) && (row != 0)) {
      out.push_back(p_s - COLS);
      if((row % 2 == 0) && (p_s % COLS != 3)) {
        out.push_back(p_s - (COLS + 1));
      } else
      if((row % 2 == 1) && (p_s % COLS != 0)) {
        out.push_back(p_s - (COLS- 1 ));
      }
    }

    return out;
}

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

const bool Checkers::checkError(const Contents& p_c) const {
  return (std::get<0>(p_c) == Player::ERROR) ||
         (std::get<1>(p_c) == Piece::ERROR);
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