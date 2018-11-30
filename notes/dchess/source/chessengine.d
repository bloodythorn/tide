module chessengine;

import std.algorithm : each;
import std.traits : EnumMembers;

import chessboard;

/** @ Brief Sets the board up for a standard start */
static void SetupBoard(ref ChessBoard p_board) {
  p_board.clear;
  [ EnumMembers!Square ][(1*8+1)..(2*8+1)].each!(
    (ref n) => p_board.setPiece(Team.White, PieceType.Pawn, n));
  [Square.A1, Square.H1].each!(
    (ref n) => p_board.setPiece(Team.White, PieceType.Rook, n));
  [Square.B1, Square.G1].each!(
    (ref n) => p_board.setPiece(Team.White, PieceType.Knight, n));
  [Square.C1, Square.F1].each!(
    (ref n) => p_board.setPiece(Team.White, PieceType.Bishop, n));
  p_board.setPiece(Team.White, PieceType.King, Square.E1);
  p_board.setPiece(Team.White, PieceType.Queen, Square.D1);
  [ EnumMembers!Square ][(6*8+1)..(7*8+1)].each!(
    (ref n) => p_board.setPiece(Team.Black, PieceType.Pawn, n));
  [Square.A8, Square.H8].each!(
    (ref n) => p_board.setPiece(Team.Black, PieceType.Rook, n));
  [Square.B8, Square.G8].each!(
    (ref n) => p_board.setPiece(Team.Black, PieceType.Knight, n));
  [Square.C8, Square.F8].each!(
    (ref n) => p_board.setPiece(Team.Black, PieceType.Bishop, n));
  p_board.setPiece(Team.Black, PieceType.King, Square.E8);
  p_board.setPiece(Team.Black, PieceType.Queen, Square.D8);
}

struct Move { };

class ChessEngine {

  public {

    this() {
//      m_currBoard = new ChessBoard(PieceRep, BoardRep);
//      m_currBoard.reset;
    }

//    dstring toDString() const pure @safe { return m_currBoard.toDString.idup; }
  }

  private {

  }
}

/* getMoves
 * getAttacks
 * checkCheck
 * getAllValidMoves
 * move
 * rollBackMove
 * rollBackMoves
 * lastMove
 * rollBackStack
 * clearRollBack
 * movesOnStack
 * validateMove
 * Check_Pawn_Files(Team)
 * Check_Pawn_Blocked(Team)
 * Check_Pawn_Isolated(Team)
 * Check_Caputre(Src, Dest)
 * Check_Castle(Team)
 * Check_EnPassant(Team)
 * Check_Threat(Trgt)
 * Move_Get_Castles(Team)
 * Move_Get_EnPassants(Team)
 * Move_Get_All(Team)
 */