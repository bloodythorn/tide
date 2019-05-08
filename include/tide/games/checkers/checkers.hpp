#ifndef CHECKERS_HPP
#define CHECKERS_HPP

#include <array>
#include <ostream>
#include <string>

#include "tide/engine/logger.hpp"
#include "tide/games/checkers/checkerboard.hpp"

namespace tide { namespace Games { namespace Checkers {

/* New Game txt string for standard English Draughts */
static const std::string ENGLISH_DR{"0bbbbbbbbbbbbxxxxxxxxrrrrrrrrrrrr"};
static constexpr std::array<char, (int)Piece::COUNT + 1> CHAR_PIECE_REP
  = { 'b', 'r', 'x' };

/** @class Options
 *
 * @ingroup Games
 *
 * This class will conduct the game for a standard
 * English Draughts/American Checkers game.
 *
 */
class Checkers {
public:

  /* Data Members */
  static const std::wstring MODULE_NAME;  /*< Module string for logging */

  /* Ctor/Xtor */
  Checkers(void);
  Checkers(const Checkers& p_ot);
  Checkers(Checkers&& p_ot);
  ~Checkers();

  /* Member Functions */

  /** @brief Save the game
   *
   * This function will return a string that when passed to the 'load' function
   * will restore the state of the game.
   *
   * @return std::string containing game state.
   */
  std::string save(void);

  /** @brief List valid moves
   *
   * This function will return a string that will contain a list of valid
   * moves. Moves will take the format of two numbers. The source square,
   * and the destination square.
   *
   * @return std::string containing game state.
   */
  std::string listMoves(void);
  std::string rlistMoves(void);
  bool newGame(void);
  bool load(const std::string&);
  bool move(Square, Square);
  bool rmove(Square, Square);

  /* Operator Overloads */
  Checkers& operator=(const Checkers& p_ot);

  Checkers& operator=(Checkers&& p_ot);

private:

  /* Data */
  CheckerBoard m_cb;
  Player       m_pl;

  /* Helper Functions */
  const Player charToPlayer(const char&) const;
  const Player charToTurn(const char&) const;
  const Piece charToPiece(const char&) const;
  const char PosToChar(const CheckerBoard&, const Square&) const;

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