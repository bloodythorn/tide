module chessboard;

import core.exception : SwitchError;
import std.traits : EnumMembers;

/** @brief Eight Bit Value */
const ubyte EIGHT_BITS = 8;

/** @brief UTF Representation Constants */
static const dchar WHITE_SQUARE = '\u25A0'; // ■
static const dchar BLACK_SQUARE = '\u25A1'; // □
static const dchar OCCUP_SQUARE = '\u25C6'; // ◆
static const dchar WHITE_KING   = '\u2654'; // ♔
static const dchar WHITE_QUEEN  = '\u2655'; // ♕
static const dchar WHITE_ROOK   = '\u2656'; // ♖
static const dchar WHITE_BISHOP = '\u2657'; // ♗
static const dchar WHITE_KNIGHT = '\u2658'; // ♘
static const dchar WHITE_PAWN   = '\u2659'; // ♙
static const dchar BLACK_KING   = '\u265A'; // ♚
static const dchar BLACK_QUEEN  = '\u265B'; // ♛
static const dchar BLACK_ROOK   = '\u265C'; // ♜
static const dchar BLACK_BISHOP = '\u265D'; // ♝
static const dchar BLACK_KNIGHT = '\u265E'; // ♞
static const dchar BLACK_PAWN   = '\u265F'; // ♟
static const dchar[2] BoardRepUTF = [ WHITE_SQUARE, BLACK_SQUARE];
static const dchar[PieceType.Max][Team.Max] PieceRepUTF = [
  [ BLACK_KING,   BLACK_QUEEN, BLACK_BISHOP,
    BLACK_KNIGHT, BLACK_ROOK,  BLACK_PAWN ],
  [ WHITE_KING,   WHITE_QUEEN, WHITE_BISHOP,
    WHITE_KNIGHT, WHITE_ROOK,  WHITE_PAWN ] ];

/** @brief 7bitASCII Representation Constants */
static const dchar[2] BoardRep = [ '+', '-' ];
static const dchar[PieceType.Max][Team.Max] PieceRep = [
  [ 'k', 'q', 'b', 'n', 'r', 'p' ],
  [ 'K', 'Q', 'B', 'N', 'R', 'P' ] ];

/** @brief Rank Constants */
enum Rank { A = 0, B, C, D, E, F, G };

/** @brief Bit Value Index for the associated Squares
  *
  * Since bit order is backwards from the physical representation, this board
  * is upside down and flipped from the physical representation. So A1 on a
  * physical board would be in the lower left, while H8 would be in the upper
  * right.
  */
enum Square {
  Er = -1,
  H1 = 0,  G1, F1, E1, D1, C1, B1, A1,
  H2,      G2, F2, E2, D2, C2, B2, A2,
  H3,      G3, F3, E3, D3, C3, B3, A3,
  H4,      G4, F4, E4, D4, C4, B4, A4,
  H5,      G5, F5, E5, D5, C5, B5, A5,
  H6,      G6, F6, E6, D6, C6, B6, A6,
  H7,      G7, F7, E7, D7, C7, B7, A7,
  H8,      G8, F8, E8, D8, C8, B8, A8,
  Max };

/** @brief Team enumeration */
enum Team { Error = -1, White = 0, Black = 1, Max = 2 };

/** @brief Piece Type enumeration */
enum PieceType {
  Error  = -1, King = 0, Queen = 1, Bishop = 2,
  Knight =  3, Rook = 4, Pawn  = 5, Max    = 6 };

/** @brief Represents a piece on the single board representation
  *
  * This struct only holds two pieces of data, Team, and PieceType.
  * Any of these two data set as Error or Max invalidates the Piece.
  *
  */
struct Piece {

  Team      team   = Team.Error;
  PieceType type   = PieceType.Error;

  /** @brief Is this piece valid?
    *
    * @return If this is a validly set piece
    */
  @property bool valid() const pure @safe {
    return !(
      (team == Team.Error)      |
      (team == Team.Max)        |
      (type == PieceType.Error) |
      (type == PieceType.Max)   );
  }
};

/* Aliases */
alias bitboard = ulong;

/* Utility Functions */

/** @brief Returns a rank mask given an uint */
static bitboard RankMask(uint p_r) { return 2^^8-1L << (8*(p_r-1)); }

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
static Square[] BitBoardToIndexes(in bitboard p_b) pure @safe {
  /** @brief DeBruijn Constants */
  const ulong deBruijn64 = 0x03f79d71b4cb0a89;
  const uint[] index64 = [
     0, 47,  1, 56, 48, 27,  2, 60,
    57, 49, 41, 37, 28, 16,  3, 61,
    54, 58, 35, 52, 50, 42, 21, 44,
    38, 32, 29, 23, 17, 11,  4, 62,
    46, 55, 26, 59, 40, 36, 15, 53,
    34, 51, 20, 43, 31, 22, 10, 45,
    25, 39, 14, 33, 19, 30,  9, 24,
    13, 18,  8, 12,  7,  6,  5, 63 ];
  Square[] output;
  bitboard board = p_b;
  while (board != 0) {
    auto result = index64 [(((board ^ (board - 1)) * deBruijn64) >> 58)];
    output ~= cast(Square)result;
    board &= board - 1;
  }
  return output;
}

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
static ubyte BitBoard_Count_Occupied(in bitboard p_b) pure @safe {
  ubyte count;
  bitboard board = p_b;
  for (count=0; board!=0; count++) board &= board-1;
  return count;
}

/** @brief Flips a bitmap on the horizontal
  *
  * This function, given a bitmap will flip it on the horizontal.
  *
  * @param p_brd bitboard that needs to be flipped.
  * @return bitboard containing board with flipped orientation.
  */
static bitboard BitBoard_Flip_Hor(bitboard p_brd) pure @safe {
  bitboard tempMask = 0;
  bitboard offsetBit = 1;
  for (int bitIdx = 0; bitIdx < Square.Max; bitIdx++) {
    /* If bit found add into tempMask and offset for the flipside of board */
    if ((p_brd & (offsetBit << bitIdx)) > 0)
      tempMask = tempMask | (offsetBit << ((Square.Max - 1) - bitIdx));
  }
  return tempMask;
}

/** @brief Flips a bitmap on the vertical
  *
  * This function, given a bitmap will flip it on the vertical.
  *
  * @param p_brd bitboard that needs to be flipped.
  * @return bitboard containing board with flipped orientation.
  */
static bitboard BitBoard_Flip_Ver(bitboard p_brd) {
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
static dstring BitBoardToString(
  in bitboard p_board,
  in dchar  p_pce = OCCUP_SQUARE,
  in dchar  p_blk = BLACK_SQUARE,
  in dchar  p_wht = WHITE_SQUARE) {

  bitboard bitMask = 0x8000000000000000;
  dchar[] op;

  for(uint bit = 0; bit < Square.Max; bit++) {

    if((p_board & bitMask) > 0) op ~= p_pce;
    else op ~= (((bit / EIGHT_BITS) % 2) == (bit % 2)) ? p_wht : p_blk;

    if((bit + 1) % EIGHT_BITS == 0) op ~= '\n';

    bitMask >>= 1;
  }

  op.length--;
  return op.idup;
}

/*TODO: Change Documentation & figger what this does */
static dstring BitBoardToStringRows(
  in ulong[] p_in,
  in size_t  p_brk = 8,
  in dchar   p_pce = OCCUP_SQUARE,
  in dchar   p_blk = BLACK_SQUARE,
  in dchar   p_wht = WHITE_SQUARE) {

  /* Nothing to do */
  if(p_in.length == 0) return "";
  else if(p_in.length == 1) return BitBoardToString(p_in[0]);

  /* Initialize */
  dchar[] op;
  dstring[] boards;
  const size_t TOP = EIGHT_BITS - 1;
  const size_t LINE_SZ = EIGHT_BITS+1; // Compensate for \n

  /* Convert to string rep */
  foreach(a; p_in) boards ~= BitBoardToString(a);

  /* Convert to a single string */
  size_t start = 0;
  size_t end = (boards.length >= p_brk) ? p_brk - 1 : boards.length - 1;

  while(start < end) {

    for(size_t rank = 0; rank <= TOP; ++rank)
      for(size_t item = start; item <= end; ++item) {
        auto bg = rank*LINE_SZ;
        auto en = rank*LINE_SZ + EIGHT_BITS;
        op ~= boards[item][bg..en];
        if (item != end) op ~= ' ';
        else op ~= '\n';
      }

      op ~= '\n';

      start += p_brk;
      end =
        ((end + p_brk) < boards.length) ?
          end + p_brk :
          boards.length - 1;
  }

  return op.idup;
}

/** @brief Chess Board Class
  *
  * Class that directly represents the chess board. It holds no rules and no
  * logic other than to represent the board, or change the board.
  */
class ChessBoard {

  public {

    /** @brief Constructor for ChessBoard
      *
      * This constructor needs two items to construct a board. It needs to know
      * how, in text, you want the board represented. It takes one 2d array of
      * size PieceType.Max, Team.Max, and a 1d array of size 2. The values of
      * both arrays have to be valid dchars.
      *
      * @param p_pRep Team Representation
      * @param p_bRep Board Representation
      */
    this(
      dchar[PieceType.Max][Team.Max] p_pRep = PieceRep,
      dchar[2] p_bRep = BoardRep) {
      m_pieceRep = p_pRep;
      m_boardRep = p_bRep;
      clear;
    }

    /** @brief Clear Board
      *
      * This function clears both board reps to empty.
      *
      */
    void clear() {
      m_bitBoards = [[0,0,0,0,0,0],[0,0,0,0,0,0]];
      foreach(a; [ EnumMembers!Square ][1..$-1]) m_squares[a] = Piece();
    }

    /** @brief Get Occupancy Board
      *
      *  Given a Team or Team.Max for both, and a Piece or Piece.Max for both,
      *  this function will return an occupancy map.
      *
      *  This function has bounds checking
      *
      * @param p_team Team to retrieve - Team.Max for both
      * @param p_piece Piece map to return.
      * @return ulong containing occupancy board requested
      */
    ulong getOccupancy(Team p_team, PieceType p_piece) {
      ulong board = 0;

      /* Recurse if Team.Max for both teams. Error on Team.Error */
      with(Team)
      if(p_team == Error) throw new SwitchError("Error Condition Hit");
      else if(p_team == Max)
        return getOccupancy(White, p_piece) | getOccupancy(Black, p_piece);

      /* Recurse if PieceType.Max for all pieces. Error on PieceType.Error */
      with(PieceType)
      if(p_piece == Error)
          throw new SwitchError("Error Condition Hit");
      else if(p_piece == Max)
        return
          getOccupancy(p_team, King  ) | getOccupancy(p_team, Queen ) |
          getOccupancy(p_team, Bishop) | getOccupancy(p_team, Knight) |
          getOccupancy(p_team, Rook  ) | getOccupancy(p_team, Pawn  ) ;

      /* Return given board */
      return m_bitBoards[p_team][p_piece];
    }

    /** @brief Detects if a square is empty.
      *
      * Given a square, this will return false if there is nothing in it.
      * True if there is.
      *
      * @param p_s Square to check
      * @return result
      */
    bool isEmpty(Square p_s) { return !m_squares[p_s].valid; }

    /** @brief Put a Piece on the Board
      *
      *  This function, given a piece team, type and location will place
      *  the piece with the stats specified. If there is another piece
      *  already in that location, it will replace it. If the location
      *  is invalid, an exception will be thrown.
      *
      * @param p_t - team to place
      * @param p_p - piece type to place
      * @param p_s - location to place on.
      */
    void setPiece(Team p_t, PieceType p_p, Square p_s) {
      /* First we make sure it's empty */
      if(!isEmpty(p_s)) unSetPiece(p_s);
      /* Get position mask for location */
      auto mask = 1L << p_s;
      /* OR mask with team/piece bitboard given */
      m_bitBoards[p_t][p_p] |= mask;
      /* Set piece on board */
      m_squares[p_s] = Piece(p_t, p_p);
    }

    /** @brief Removes Piece from Board
      *
      *  Given a location will remove any piece in that location from the
      *  board. It will throw an exception if the location is invalid, or
      *  if the piece does not exist nothing.
      *
      * @param p_s coordinate for the removal
      */
    void unSetPiece(Square p_s) {
      /* IF already empty, nothing to do */
      if(isEmpty(p_s)) return;
      /* Get position mask for location */
      auto mask = 1L << p_s;
      /* XOR mask to remove that location from bitboard. */
      m_bitBoards[m_squares[p_s].team][m_squares[p_s].type] ^= mask;
      /* Clear piece from board */
      m_squares[p_s] = Piece();
    }

    /** @brief Returns piece at given location
      *
      * This function, given a square will return a Piece struct containing
      * information on the piece in the given square.
      *
      * @param p_s Square to retrieve
      * @return Piece struct with piece from given square
      */
    Piece getPiece(Square p_s) { return m_squares[size_t(p_s)]; }

    /** @brief Outputs a string rep
      *
      * This function will output a string representation of the
      * chess board.
      *
      */
    dstring toDString() const pure @safe {
      dchar[] op;
      foreach(i, a; m_squares) {
        if(a.valid) op = m_pieceRep[a.team][a.type] ~ op;
        else
          op =
            ((((i / EIGHT_BITS) % 2) == (i % 2)) ?
              m_boardRep[0] :
              m_boardRep[1])
            ~ op;
        if((i + 1) % EIGHT_BITS == 0) op = '\n' ~ op;
      }
      return op[1..$].idup;
    }

    /** @brief Equals Operator */
    bool opEquals()(auto ref const ChessBoard rhs ) const {
      return false;
    }
  }

  private {

    /** @brief Graphic Board Representation */
    dchar[PieceType.Max][Team.Max] m_pieceRep;
    dchar[Team.Max]                m_boardRep;

    /** @brief Physical Board Representations */
    Piece[Square.Max]              m_squares;
    ulong[PieceType.Max][Team.Max] m_bitBoards = [[0,0,0,0,0,0],[0,0,0,0,0,0]];
  }
}