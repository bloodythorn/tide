#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP

#include <array> //std::array
#include <string> //string
#include <vector> //vector

#include <tide/engine/logger.hpp>

namespace tide { namespace Games { namespace Chess {

/* Module Name */
const std::wstring MODULE_NAME{L"Chess"};

/** @brief Eight Bit Value */
static const short EIGHT_BITS{8};
static const short TWO_FIVE_FIVE{255};

/** @brief Rank Constants */
enum class Rank { A = 0, B, C, D, E, F, G };

/** @brief Bit Value Index for the associated Squares
  *
  * Since bit order is backwards from the physical representation, this board
  * is upside down and flipped from the physical representation. So A1 on a
  * physical board would be in the lower left, while H8 would be in the upper
  * right.
  */
enum class Square {
  Error = -1,
  H1 = 0,  G1, F1, E1, D1, C1, B1, A1,
  H2,      G2, F2, E2, D2, C2, B2, A2,
  H3,      G3, F3, E3, D3, C3, B3, A3,
  H4,      G4, F4, E4, D4, C4, B4, A4,
  H5,      G5, F5, E5, D5, C5, B5, A5,
  H6,      G6, F6, E6, D6, C6, B6, A6,
  H7,      G7, F7, E7, D7, C7, B7, A7,
  H8,      G8, F8, E8, D8, C8, B8, A8,
  Max };

/** @brief Player enumeration */
enum class Player { Error = -1, White = 0, Black = 1, Max = 2 };

/** @brief Type enumeration */
enum class Piece {
  Error  = -1, King = 0, Queen = 1, Bishop = 2,
  Knight =  3, Rook = 4, Pawn  = 5, Max    = 6 };

inline const bool isValid(Square sq) {
  return (sq != Square::Error && sq != Square::Max);
}
inline const bool isValid(Player pl) {
  return (pl != Player::Error && pl != Player::Max);
}
inline const bool isValid(Piece pc) {
  return (pc != Piece::Error && pc != Piece::Max);
}

/* Aliases */
using bitboard = std::uint64_t;
using bitboards =
  std::array<
    std::array<
      bitboard,
      static_cast<unsigned int>(Chess::Piece::Max)>,
    static_cast<unsigned int>(Chess::Player::Max)>;

/* Utility Functions */

/** @brief Returns a rank mask given an uint */
bitboard RankMask(unsigned int p_rank);

/** @brief Convert ulong board into an index list
  *
  * Given a ulong representation of a bitboard, this function will return an
  * array of Square indexes of each location that is populated. This function
  * uses deBruijn64 generated numbers to accomplish this and is only faster
  * than linear search if the board is less than 50% populated.
  *
  * @param p_b Bitboard ulong
  * @return Square[] with a list of all populated indexes.
  */
std::vector<Square> BitBoardToIndexes(const bitboard& p_b);

/** @brief Counts Population on BitBoards
 *
 *  This function, when given a bitboard will count the population
 *  on it and return the count.
 *
 *  On the Maths: The 'board-1' operation creates a new mask that
 *  has the bit on in every index from 0 to the first occurrence
 *  of an on bit. When &ed with the original board, it produces a
 *  new board, minus that occurrence. Finally, the final board it
 *  will produce will be zero, the terminal condition.
 *
 *  @param p_b The board to be counted.
 *  @return ubyte with pop-count.
 */
std::uint8_t CountOccupied(bitboard p_b);

/** @brief Flips a bitmap on the horizontal
  *
  * This function, given a bitmap will flip it on the horizontal.
  *
  * @param p_brd bitboard that needs to be flipped.
  * @return bitboard containing board with flipped orientation.
  */
bitboard FlipHorizontal(bitboard p_brd);

/** @brief Flips a bitmap on the vertical
  *
  * This function, given a bitmap will flip it on the vertical.
  *
  * @param p_brd bitboard that needs to be flipped.
  * @return bitboard containing board with flipped orientation.
  */
bitboard FlipVertical(bitboard p_brd);

inline const bitboard GetMask(Square p_sq) {
  return bitboard(1) << static_cast<uint64_t>(p_sq);
}

/* Board Printing Functions */

/** @brief Converts a board to a string representation
  *
  * Given a ulong bitboard, a dchar rep for an occupied square, a black square
  * and a whilte square, it will return a dstring representation of the given
  * bitboard. Troubleshooting tool.
  *
  * @param p_board bitboard ulong
  * @param p_pce Occupied square rep
  * @param p_blk Black square rep
  * @param p_wht White square rep
  * @return dstring rep of occupancy
  */
std::wstring BitBoardToString(
  bitboard p_board,
  wchar_t  p_pce = L'#',
  wchar_t p_blk = L'X',
  wchar_t p_wht = L'O');

/** @brief Converts multiple boards to a string representation
  *
  * Given a vector of bitboards, break length and character representation,
  * this function will give all boards in a row, adhering to the break given.
  * Troubleshooting tool.
  *
  * @param p_bvec Vector of bitboards
  * @param p_brk  Break Width
  * @param p_pce  Occupied square rep
  * @param p_blk  Black square rep
  * @param p_wht  White square rep
  * @return wstring rep of occupancy
  */
std::wstring BitBoardToStringRows(
  const std::vector<bitboard>& p_bvec,
  size_t  p_brk = 8,
  wchar_t p_pce = L'#',
  wchar_t p_blk = L'X',
  wchar_t p_wht = L'O');

class ChessBoard {
public:

  /* Ctor/Xtor */
  explicit ChessBoard(void);
  ChessBoard(const ChessBoard& p_ot);
  ChessBoard(ChessBoard&& p_ot);
  ~ChessBoard();

  /* Operator Overloads */
  ChessBoard& operator=(const ChessBoard& p_ot);
  ChessBoard& operator=(ChessBoard&& p_ot);

  /* Member Functions */

  void     set(Player p_pl, Piece p_pc, Square p_sq);
  void     unset(Square p_sq);
  void     clear(void);

  bitboard getOccupancy(Player p_pl, Piece p_pc);
  Piece    getPiece(Square p_sq);
  Player   getPlayer(Square p_sq);
  bool     isEmpty(Square p_sq);

private:

  /* Data Members */

  bitboards m_boards{};

  friend std::ostream& operator<<(std::ostream& p_os, const ChessBoard& p_ot) {
    p_os << std::string("Hello");
    return p_os;
  }
  friend std::wostream& operator<<(std::wostream& p_os, const ChessBoard& p_ot) {
    p_os << std::wstring(L"WHello");
    return p_os;
  }

};

}/*Chess*/}/*games*/}/*tide*/


#endif //CHESSBOARD_HPP