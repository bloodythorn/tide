using System.Collections.Generic;
using UvsChess;
using System;

namespace GroupEight
{
    public class GreedyAI : BaseAI, IChessAI
    {
        #region IChessAI Members that are implemented by the Student

        Random Rng = new Random();

        public GreedyAI()
        {

        }

        /// <summary>
        /// The name of your AI
        /// </summary>
        public string Name
        {
#if DEBUG
            get { return "GroupEight_GreedyAI (Debug)"; }
#else
            get { return "GroupEight_GreedyAI"; }
#endif
        }

        /// <summary>
        /// Evaluates the chess board and decided which move to make. This is the main method of the AI.
        /// The framework will call this method when it's your turn.
        /// </summary>
        /// <param name="board">Current chess board</param>
        /// <param name="myColor">Your color</param>
        /// <returns> Returns the best chess move the player has for the given chess board</returns>
        public ChessMove GetNextMove(ChessBoard board, ChessColor myColor)
        {
            var bitBoard = new ChessBitBoard(board, Log);
            var ourTeam = myColor == ChessColor.Black ? Team.Black : Team.White;
            var enemyTeam = ourTeam == Team.White ? Team.Black : Team.White;

            var allMoves = new Queue<Move>();
            var allEnemyMoves = new Queue<Move>();

            bitBoard.getAllValidMoves(ourTeam, allMoves);

            if (IsStalemate(bitBoard, allMoves, enemyTeam))
            {
                return new ChessMove(new ChessLocation(0, 0), new ChessLocation(0, 0), ChessFlag.Stalemate);
            }

            var moveToMake = MoveChooser(ourTeam, bitBoard, allMoves);

            var flag = GetMoveFlag(bitBoard, moveToMake, enemyTeam);

            // convert from our cartesian coordinates to the frameworks
            var srcy = 7 - moveToMake.srce.Y;
            var desty = 7 - moveToMake.dest.Y;

            return new ChessMove(new ChessLocation(moveToMake.srce.X, srcy), new ChessLocation(moveToMake.dest.X, desty), flag);
        }

        /* Here's where I weight it */
        int weighMove(Move move, ChessBitBoard board, Team team)
        {
            const int CAUSE_THREAT = 10;
            const int CHECK = 1;
            const int PAWN_QUEE = 10;
            const int PAWN_RANK_MULT = 1;
            const int CHECK_MATE = 100;
            /*Piece Weight in relation to the enum */
            /* King, Queen, Bishop, Knight, Rook, Pawn */
            var pieceCost = new int[] { 20, 13, 11, 11, 12, 10 };
            var piece = board.getPieceType(move.srce.X, move.srce.Y);
            var enemyTeam = team == Team.White ? Team.Black : Team.White;
            var enemyPieceCount = ChessBitBoard.countBoardPop(board.getOccupancy(enemyTeam, Piece.Max));
            var ourPieceCount = ChessBitBoard.countBoardPop(board.getOccupancy(team, Piece.Max));
            var weHaveAdvantage = ourPieceCount - enemyPieceCount > 2;

            int SRC_THREAT = pieceCost[(int)piece];
            int DEST_THREAT = pieceCost[(int)piece] * -2;

            int score = 0;
            if (move.capture != Piece.Error)
                score += pieceCost[(int)move.capture];
            if (move.causesCheck)
                score += CHECK;
            if (move.pawnToQueen)
                score += PAWN_QUEE;
            if (move.srcUnderThreat)
                score += SRC_THREAT;
            if (move.destUnderThreat)
                score += DEST_THREAT;
            if (move.causesEnemyThreat)
            {
                if (weHaveAdvantage && enemyPieceCount < 10 && Rng.Next(1, 3) % 2 == 0)
                {
                    score += 2;
                }
                score += CAUSE_THREAT;
            }
            if (piece == Piece.Pawn)
            {
                if (weHaveAdvantage && enemyPieceCount < 6)
                {
                    score += 15;
                }
                score += PAWN_RANK_MULT * board.pawnCountForRank(team, move.srce.Y);
            }

            board.move(move.srce.X, move.srce.Y, move.dest.X, move.dest.Y);

            if (move.causesCheck)
            {
                Queue<Move> opMoves = new Queue<Move>();
                board.getAllValidMoves(enemyTeam, opMoves);
                if (opMoves.Count == 0)
                    score += CHECK_MATE;
            }
            board.rollBackMove();

            return score;
        }

        /* This is where you set which AI you want choosing moves */
        Move MoveChooser(Team team, ChessBitBoard board, Queue<Move> moves)
        {
            var bestWeight = int.MinValue;
            Move bestMove = null;

            foreach (var move in moves)
            {
                var moveWeight = weighMove(move, board, team);
                if (moveWeight > bestWeight)
                {
                    bestMove = move;
                    bestWeight = moveWeight;
                }
            }

            return bestMove;
        }

        /// <summary>
        /// Validates a move. The framework uses this to validate the opponents move.
        /// </summary>
        /// <param name="boardBeforeMove">The board as it currently is _before_ the move.</param>
        /// <param name="moveToCheck">This is the move that needs to be checked to see if it's valid.</param>
        /// <param name="colorOfPlayerMoving">This is the color of the player who's making the move.</param>
        /// <returns>Returns true if the move was valid</returns>
        public bool IsValidMove(ChessBoard boardBeforeMove, ChessMove moveToCheck, ChessColor colorOfPlayerMoving)
        {
            var bitBoard = new ChessBitBoard(boardBeforeMove, Log);
            return ValidateMove(bitBoard, moveToCheck, colorOfPlayerMoving);
        }

        #endregion

        #region IChessAI Members that should be implemented as automatic properties and should NEVER be touched by students.
        /// <summary>
        /// This will return false when the framework starts running your AI. When the AI's time has run out,
        /// then this method will return true. Once this method returns true, your AI should return a 
        /// move immediately.
        /// 
        /// You should NEVER EVER set this property!
        /// This property should be defined as an Automatic Property.
        /// This property SHOULD NOT CONTAIN ANY CODE!!!
        /// </summary>
        public AIIsMyTurnOverCallback IsMyTurnOver { get; set; }

        /// <summary>
        /// Call this method to print out debug information. The framework subscribes to this event
        /// and will provide a log window for your debug messages.
        /// 
        /// You should NEVER EVER set this property!
        /// This property should be defined as an Automatic Property.
        /// This property SHOULD NOT CONTAIN ANY CODE!!!
        /// </summary>
        public AILoggerCallback Log { get; set; }

        /// <summary>
        /// Call this method to catch profiling information. The framework subscribes to this event
        /// and will print out the profiling stats in your log window.
        /// 
        /// You should NEVER EVER set this property!
        /// This property should be defined as an Automatic Property.
        /// This property SHOULD NOT CONTAIN ANY CODE!!!
        /// </summary>
        public AIProfiler Profiler { get; set; }

        /// <summary>
        /// Call this method to tell the framework what decision print out debug information. The framework subscribes to this event
        /// and will provide a debug window for your decision tree.
        /// 
        /// You should NEVER EVER set this property!
        /// This property should be defined as an Automatic Property.
        /// This property SHOULD NOT CONTAIN ANY CODE!!!
        /// </summary>
        public AISetDecisionTreeCallback SetDecisionTree { get; set; }
        #endregion
    }
}
