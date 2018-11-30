using System;
using System.Linq;
using UvsChess;
using System.Reflection;
using System.Collections.Generic;
using System.IO;

namespace GroupEight
{
    public static class Extensions
    {
        public static ChessBoard ToChessBoard(this ChessBitBoard bitBoard)
        {
            var chessBoard = new ChessBoard();

            for (int x = 0; x < 8; ++x)
            {
                for (int y = 0; y < 8; ++y)
                {
                    var pieceTeam = bitBoard.getPieceTeam(x, y);
                    var pieceType = bitBoard.getPieceType(x, y);

                    var y2 = 7 - y;

                    if (pieceTeam == Team.Black)
                    {
                        switch (pieceType)
                        {
                            case Piece.Bishop:
                                chessBoard[x, y2] = ChessPiece.BlackBishop;
                                break;
                            case Piece.King:
                                chessBoard[x, y2] = ChessPiece.BlackKing;
                                break;
                            case Piece.Knight:
                                chessBoard[x, y2] = ChessPiece.BlackKnight;
                                break;
                            case Piece.Pawn:
                                chessBoard[x, y2] = ChessPiece.BlackPawn;
                                break;
                            case Piece.Queen:
                                chessBoard[x, y2] = ChessPiece.BlackQueen;
                                break;
                            case Piece.Rook:
                                chessBoard[x, y2] = ChessPiece.BlackRook;
                                break;
                            default:
                                chessBoard[x, y2] = ChessPiece.Empty;
                                break;
                        }
                    }
                    else
                    {
                        switch (pieceType)
                        {
                            case Piece.Bishop:
                                chessBoard[x, y2] = ChessPiece.WhiteBishop;
                                break;
                            case Piece.King:
                                chessBoard[x, y2] = ChessPiece.WhiteKing;
                                break;
                            case Piece.Knight:
                                chessBoard[x, y2] = ChessPiece.WhiteKnight;
                                break;
                            case Piece.Pawn:
                                chessBoard[x, y2] = ChessPiece.WhitePawn;
                                break;
                            case Piece.Queen:
                                chessBoard[x, y2] = ChessPiece.WhiteQueen;
                                break;
                            case Piece.Rook:
                                chessBoard[x, y2] = ChessPiece.WhiteRook;
                                break;
                            default:
                                chessBoard[x, y2] = ChessPiece.Empty;
                                break;
                        }
                    }
                }
            }

            return chessBoard;
        }

        public static ChessFlag GetMoveFlag(ChessBitBoard bitBoard, Move move, Team enemyTeam)
        {
            var flag = ChessFlag.NoFlag;
            var allEnemyMoves = new Queue<Move>();

            if (move.causesCheck)
            {
                bitBoard.move(move.srce.X, move.srce.Y, move.dest.X, move.dest.Y);
                bitBoard.getAllValidMoves(enemyTeam, allEnemyMoves);

                if (allEnemyMoves.Count == 0)
                {
                    flag = ChessFlag.Checkmate;
                }
                else
                {
                    flag = ChessFlag.Check;
                }
            }

            return flag;
        }

        public static ChessMove ToChessMove(this Move move, ChessBitBoard board, Team team)
        {
            var enemyTeam = team == Team.White ? Team.Black : Team.White;
            var flag = GetMoveFlag(board, move, enemyTeam);

            var srcy = 7 - move.srce.Y;
            var desty = 7 - move.dest.Y;

            return new ChessMove(new ChessLocation(move.srce.X, srcy), new ChessLocation(move.dest.X, desty), flag);
        }

        public static ChessColor ToChessColor(this Team team)
        {
            return team == Team.White ? ChessColor.White : ChessColor.Black;
        }
    }

    public class ChessAI
    {
        IChessAI ai;

        public ChessAI(string aiName)
        {
            List<string> aiNames = new List<string>();

            foreach (var dllFilePath in Directory.GetFiles(Environment.CurrentDirectory, "*.dll"))
            {
                var asm = Assembly.LoadFrom(dllFilePath);

                foreach (var type in asm.GetTypes())
                {
                    if (type.GetInterfaces().Contains(typeof(IChessAI)))
                    {
                        var fwai = (IChessAI)Activator.CreateInstance(type);
                        aiNames.Add(fwai.Name);
                        if (fwai.Name.Equals(aiName))
                        {
                            ai = fwai;
                            break;
                        }
                    }
                }
            }

            if (ai == null)
            {
                throw new Exception($"Could not find AI {aiName}. Here's a list of the available ai's:\n{string.Join("\n", aiNames)}");
            }
        }

        public static void listAllAi()
        {
            foreach (var dllFilePath in Directory.GetFiles(Environment.CurrentDirectory, "*.dll"))
            {
                var asm = Assembly.LoadFrom(dllFilePath);
                var asmPrinted = false;

                foreach (var type in asm.GetTypes())
                {
                    if (type.GetInterfaces().Contains(typeof(IChessAI)))
                    {
                        if (!asmPrinted)
                        {
                            Console.WriteLine(Path.GetFileName(dllFilePath));
                            asmPrinted = true;
                        }

                        var fwai = (IChessAI)Activator.CreateInstance(type);
                        Console.WriteLine($"\t{fwai.Name}");
                    }
                }
            }
        }

        public bool isValidMove(ChessBitBoard board, Move move, Team team)
        {
            return ai.IsValidMove(board.ToChessBoard(), move.ToChessMove(board, team), team.ToChessColor());
        }

        public Move getNextMove(ChessBitBoard board, Team team)
        {
            var chessMove = ai.GetNextMove(board.ToChessBoard(), team.ToChessColor());

            var srcy = 7 - chessMove.From.Y;
            var desty = 7 - chessMove.To.Y;

            board.move(chessMove.From.X, srcy, chessMove.To.X, desty);
            var move = board.lastMove();
            board.rollBackMove();

            return move;
        }
    }
}
