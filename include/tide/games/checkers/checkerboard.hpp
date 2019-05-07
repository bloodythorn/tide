#ifndef CHECKERBOARD_HPP
#define CHECKERBOARD_HPP

#include <array>
#include <ostream>
#include <string>

namespace tide { namespace Games { namespace Checkers {

/* Enums and aliases */
enum class Player { BLACK, RED, COUNT };
enum class Piece { MAN, KING, COUNT };
using Square = uint8_t;
static const Square SQUARE_COUNT{32};

class CheckerBoard {
public:

  /* Ctor/Xtor */
  CheckerBoard();

  CheckerBoard(const CheckerBoard& p_ot);

  CheckerBoard(CheckerBoard&& p_ot);

  ~CheckerBoard();

  /* Methods */

  /** @brief Clear a Square
   *
   * This function will unset any particular square.
   *
   * @param Square which square you need cleared.
   * @return success/fail.
   */
  void clear(Square);

  /** @brief Set a Square
   *
   * This function will set a square to a specific player/piece combo.
   *
   * @param Square which square you need set.
   * @param Player What player you want it set to.
   * @param man or king?
   * @return success/fail.
   */
  void set(Square, Player, Piece);

  /** @brief Retrieves player
   *
   * Given a square this will retrieve which player owns the piece
   * on this square. It will return COUNT for a clear square.
   *
   * @param Square which square you need checked.
   * @return Piece containing which team the square belongs to.
   */
  const bool getPlayer(Square) const;

  /** @brief Retrieves if piece is a King or not.
   *
   * Given a square this will retrieve which player owns the piece
   * on this square. It will return COUNT for a clear square.
   *
   * @param Square which square you need checked.
   * @return Piece containing which team the square belongs to.
   */
  const bool isKing(Square) const;

  /* Operator Overloads */
  CheckerBoard& operator=(const CheckerBoard& p_ot);

  CheckerBoard& operator=(CheckerBoard&& p_ot);

private:

  /* Data */
  std::array<Piece, SQUARE_COUNT> board;

  /* Friend Functions for Streams */
  friend std::ostream& operator<<(std::ostream& p_os, const CheckerBoard& p_ot) {
    p_os << typeid(p_ot).name();
    return p_os;
  }
  friend std::wostream& operator<<(std::wostream& p_os, const CheckerBoard& p_ot) {
    p_os << typeid(p_ot).name();
    return p_os;
  }
};

}/*Checkers*/}/*Games*/}/*tide*/

#endif //CHECKERBOARD_HPP