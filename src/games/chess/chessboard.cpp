#include "tide/games/chess/chessboard.hpp"

#include <algorithm> // std::copy
#include <stdexcept> // std::out_of_range

namespace tide { namespace Games { namespace Chess {

bitboard RankMask(unsigned int p_rank) {
  return (TWO_FIVE_FIVE) << (EIGHT_BITS*(p_rank-1));
}

std::vector<Square> BitBoardToIndexes(
  const bitboard& p_b) {
  /** @brief DeBruijn Constants */
  static const std::uint64_t deBruijn64 = 0x03f79d71b4cb0a89;
  static const std::uint64_t index64[] {
     0, 47,  1, 56, 48, 27,  2, 60,
    57, 49, 41, 37, 28, 16,  3, 61,
    54, 58, 35, 52, 50, 42, 21, 44,
    38, 32, 29, 23, 17, 11,  4, 62,
    46, 55, 26, 59, 40, 36, 15, 53,
    34, 51, 20, 43, 31, 22, 10, 45,
    25, 39, 14, 33, 19, 30,  9, 24,
    13, 18,  8, 12,  7,  6,  5, 63 };
  std::vector<Square> output;
  bitboard board = p_b;
  std::uint64_t result = index64 [(((board ^ (board - 1)) * deBruijn64) >> 58)];
  while (board != 0) {
    output.push_back(static_cast<Square>(result));
    board &= board - 1;
    result = index64 [(((board ^ (board - 1)) * deBruijn64) >> 58)];
  }
  return output;
}

std::uint8_t CountOccupied(bitboard p_b) {
  std::uint8_t count;
  bitboard board = p_b;
  for (count=0; board!=0; count++) board &= board-1;
  return count;
}

bitboard FlipHorizontal(bitboard p_brd) {
  bitboard tempMask = 0;
  bitboard offsetBit = 1;
  auto max = static_cast<int>(Square::Max);
  for (int bitIdx = 0; bitIdx < max; bitIdx++) {
    /* If bit found add into tempMask and offset for the flipside of board */
    if ((p_brd & (offsetBit << bitIdx)) > 0)
      tempMask = tempMask | (offsetBit << ((max - 1) - bitIdx));
  }
  return tempMask;
}

bitboard FlipVertical(bitboard p_brd) {
  bitboard output = 0;
  const int BOT = 0;
  const int TOP = EIGHT_BITS-1;

  for(int i = BOT; i <= TOP; ++i) {
    bitboard temp = RankMask(i) & p_brd;
    if(i < 4)
      output |= temp << (EIGHT_BITS * ((TOP - i) - i));
    else
      output |= temp >> (EIGHT_BITS * (i-(TOP-i)));
  }
  return output;
}


std::wstring BitBoardToString(
  bitboard p_board,
  wchar_t  p_pce,
  wchar_t  p_blk,
  wchar_t  p_wht) {

  bitboard bitMask = 0x8000000000000000;
  std::wstring op;

  auto max = static_cast<uint8_t>(Square::Max);
  for(uint8_t bit = 0; bit < max; bit++) {

    if((p_board & bitMask) > 0) op += p_pce;
    else op += (((bit / EIGHT_BITS) % 2) == (bit % 2)) ? p_wht : p_blk;

    if((bit + 1) % EIGHT_BITS == 0) op += '\n';

    bitMask >>= 1;
  }

  op.pop_back();
  return op;
}

std::wstring BitBoardToStringRows(
  const std::vector<bitboard>& p_bvec,
  size_t  p_brk,
  wchar_t p_pce,
  wchar_t p_blk,
  wchar_t p_wht) {

  /* Nothing to do */
  if(p_bvec.size() == 0) return L"";
  else if(p_bvec.size() == 1) return BitBoardToString(p_bvec.front());

  /* Initialize */
  std::wstring op;
  std::vector<std::wstring> boards;
  const size_t TOP = EIGHT_BITS - 1;
  const size_t LINE_SZ = EIGHT_BITS+1; // Compensate for \n

  /* Convert to string rep */
  for(const auto& a : p_bvec) boards.push_back(BitBoardToString(a));

  /* Convert to a single string */
  size_t start = 0;
  size_t end = (boards.size() >= p_brk) ? p_brk - 1 : boards.size() - 1;

  while(start < end) {
    for(size_t rank = 0; rank <= TOP; ++rank)
      for(size_t item = start; item <= end; ++item) {
        auto bg = rank*LINE_SZ;
        auto en = rank*LINE_SZ + EIGHT_BITS;
        op.append(boards[item].substr(bg, en-bg));
        if(item != end) op.append(L" ");
        else op.append(L"\n");
      }
    op.append(L"\n");
    start += p_brk;
    end = ((end + p_brk) < boards.size()) ? end + p_brk : boards.size() - 1;
  }
  return op;
}

ChessBoard::ChessBoard(void) { clear(); }

ChessBoard::ChessBoard(const ChessBoard& p_ot) {
  std::copy(p_ot.m_boards.begin(), p_ot.m_boards.end(), m_boards.begin());
}

ChessBoard::ChessBoard(ChessBoard&& p_ot) {
  std::copy(p_ot.m_boards.begin(), p_ot.m_boards.end(), m_boards.begin());
}

ChessBoard::~ChessBoard() { }

ChessBoard& ChessBoard::operator=(const ChessBoard& p_ot) {
  if(this != &p_ot) {
    std::copy(p_ot.m_boards.begin(), p_ot.m_boards.end(), m_boards.begin());
  }
  return *this;
}

ChessBoard& ChessBoard::operator=(ChessBoard&& p_ot) {
  if(this != &p_ot) {
    clear();
    std::copy(p_ot.m_boards.begin(), p_ot.m_boards.end(), m_boards.begin());
    p_ot.clear();
  }
  return *this;
}

void ChessBoard::clear(void) { m_boards = {}; }

bitboard ChessBoard::getOccupancy(Player p_pl, Piece p_pc) {
  /* Handle special condition in Player */
  if(p_pl == Player::Error) return 0;
  else if(p_pl == Player::Max)
    return getOccupancy(Player::White, p_pc) |
           getOccupancy(Player::Black, p_pc);

  /* Handle special condition in Piece */
  if(p_pc == Piece::Error) return 0;
  else if(p_pc == Piece::Max)
    return getOccupancy(p_pl, Piece::King) |
           getOccupancy(p_pl, Piece::Queen) |
           getOccupancy(p_pl, Piece::Bishop) |
           getOccupancy(p_pl, Piece::Knight) |
           getOccupancy(p_pl, Piece::Rook) |
           getOccupancy(p_pl, Piece::Pawn);

  return m_boards[static_cast<int>(p_pl)][static_cast<int>(p_pc)];
}

Piece ChessBoard::getPiece(Square p_sq) {
  return Piece::Error;
}

Player ChessBoard::getPlayer(Square p_sq) {
  return Player::Error;
}

bool ChessBoard::isEmpty(Square p_sq) {
  return (getOccupancy(Player::Max, Piece::Max) & GetMask(p_sq)) == 0;
}

void ChessBoard::set(Player p_pl, Piece p_pc, Square p_sq) {
  if(!isValid(p_pl) || !isValid(p_pc) || !isValid(p_sq))
    throw std::out_of_range(
      "Player/Piece/Square Error or Max in ChessBoard::set");
  m_boards[static_cast<int>(p_pl)][static_cast<int>(p_pc)] |= GetMask(p_sq);
}

void ChessBoard::unset(Square p_sq) {
  if(!isValid(p_sq))
    throw std::out_of_range("Square Error or Max in ChessBoard::unset");
  auto mask = GetMask(p_sq);
  for(auto& a : m_boards)
    std::transform(a.begin(), a.end(), a.begin(),
      [p_sq, mask](const bitboard& b) -> bitboard { return (b & !mask); });
}

}/*Chess*/}/*Games*/}/*tide*/