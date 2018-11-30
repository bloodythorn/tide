using System.Collections.Generic;
using UvsChess;

namespace GroupEight
{
    public abstract class BaseAI
    {
        public bool ValidateMove(ChessBitBoard bitBoard, ChessMove moveToCheck, ChessColor colorOfPlayerMoving)
        {
            // When playing against an AI opponent, the framework will only call this method
            // if the AI didn't declare a stalemate or go over its time. This means we're
            // guaranteed to have a move and only need to check for check and stalemate.

            // We'll still check for stalemate in case it's a human player though.
            if (moveToCheck.Flag == ChessFlag.Stalemate)
            {
                var allWhiteMoves = new Queue<Move>();
                var allBlackMoves = new Queue<Move>();

                bitBoard.getAllValidMoves(Team.White, allWhiteMoves);
                bitBoard.getAllValidMoves(Team.Black, allBlackMoves);

                return allWhiteMoves.Count == 0 && allBlackMoves.Count == 0;
            }

            // convert from the frameworks coordinates to cartesian
            var srcy = 7 - moveToCheck.From.Y;
            var desty = 7 - moveToCheck.To.Y;

            var team = colorOfPlayerMoving == ChessColor.White ? Team.White : Team.Black;
            var enemyTeam = team == Team.White ? Team.Black : Team.White;
            var actualTeam = bitBoard.getPieceTeam(moveToCheck.From.X, srcy);

            if (team != actualTeam)
            {
                return false;
            }
           
            if (!bitBoard.validateMove(moveToCheck.From.X, srcy, moveToCheck.To.X, desty))
            {
                return false;
            }

            bitBoard.move(moveToCheck.From.X, srcy, moveToCheck.To.X, desty);

            if (bitBoard.lastMove().causesCheck)
            {
                var allEnemyMoves = new Queue<Move>();
                bitBoard.getAllValidMoves(enemyTeam, allEnemyMoves);

                if (allEnemyMoves.Count == 0)
                {
                    return moveToCheck.Flag == ChessFlag.Checkmate;
                }
                else
                {
                    return moveToCheck.Flag == ChessFlag.Check;
                }
            }

            return moveToCheck.Flag == ChessFlag.NoFlag;
        }

        public bool IsStalemate(ChessBitBoard bitBoard, Queue<Move> allMyMoves, Team enemyTeam)
        {
            var allEnemyMoves = new Queue<Move>();

            if (allMyMoves.Count == 0)
            {
                bitBoard.getAllValidMoves(enemyTeam, allEnemyMoves);

                if (allEnemyMoves.Count == 0)
                {
                    return true;
                }
            }

            return false;
        }

        public ChessFlag GetMoveFlag(ChessBitBoard bitBoard, Move move, Team enemyTeam)
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
    }
}
