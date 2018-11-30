module tests;

//import core.exception;
import std.algorithm : each, fold, map;
import std.conv : to;
import std.math : pow;
import std.stdio : writeln, write;
import std.exception : assertThrown;
import std.format : format;
import std.random : uniform;
import std.range : repeat, take, enumerate, recurrence;
import std.traits : EnumMembers;

import chessboard;
import chessengine;
import masks;



unittest {

  //auto seq = (a, b) => a[b-1]+1;

  writeln(recurrence!((a, b) => a[b-1]+1)(1).take(5));

  //ChessBoard board1 = new ChessBoard(PieceRepUTF, BoardRepUTF);
  //ChessBoard board2 = new ChessBoard(PieceRepUTF, BoardRepUTF);
  //SetupBoard(board1);
  //SetupBoard(board2);


  //[EnumMembers!PieceType][1..$-1].fold!()

  //writeln(
  //  ((board1.getOccupancy(Team.Max, PieceType.Pawn) ^
  //            board2.getOccupancy(Team.Max, PieceType.Pawn)) == 0) );
}

void shuffleBoard(ref ChessBoard p_b, int p_n) {
  /* We don't want to repeat squares so let's keep a list of squares that we've
   * shuffled */
  /* Next we'll pull a random square out n times in a for loop. */
  /* Each n, we'll
   *   Remove that square from the list of untouched squares.
   *   generate a random team.
   *   generate a random piece
   *   set that piece.
   */
}

void cloneBoard(const ref ChessBoard p_f, ref ChessBoard p_t) {
  /* This one we will probably have to:
   *   clear the board
   *   pull each occupancy map by team/piece
   *   turn it into a list of indexes.
   *   Set that piece. We can probably do this in a nested loop, or maybe
   *     even recursively.
   */
}