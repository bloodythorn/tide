#include "tide/games/chess/chessboard.hpp"

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
  wchar_t p_blk,
  wchar_t p_wht) {

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


}/*Chess*/}/*Games*/}/*tide*/