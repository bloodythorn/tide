using System.Collections.Generic;
using System.Linq;
using UvsChess;
using System;
using System.Diagnostics;

namespace GroupEight
{
    public class NegamaxAI : BaseAI, IChessAI
    {
        #region IChessAI Members that are implemented by the Student

        Random Rng = new Random();

        private const int MAX_MS = 4500;

        public NegamaxAI()
        {

        }

        /// <summary>
        /// The name of your AI
        /// </summary>
        public string Name
        {
#if DEBUG
            get { return "GroupEight_Negamax (Debug)"; }
#else
            get { return "GroupEight_Negamax"; }
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

            var moveToMake = MoveChooser(bitBoard, ourTeam, allMoves, 2);

            var flag = GetMoveFlag(bitBoard, moveToMake, enemyTeam);

            // convert from our cartesian coordinates to the frameworks
            var srcy = 7 - moveToMake.srce.Y;
            var desty = 7 - moveToMake.dest.Y;

            return new ChessMove(new ChessLocation(moveToMake.srce.X, srcy), new ChessLocation(moveToMake.dest.X, desty), flag);
        }

        /* Here's where I weight it */
        int weighMove(Team team, ChessBitBoard board, Move move)
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

        Move MoveChooser(
            ChessBitBoard board,
            Team team,
            Queue<Move> moves,
            int depth)
        {
            List<Move> sorting = new List<Move>();
            List<Move> done = new List<Move>();
            board.getAllValidMoves(team, moves);
            while (moves.Count != 0)
            {
                moves.Peek().mValue = weighMove(team, board, moves.Peek());
                sorting.Add(moves.Dequeue());
            }
            sorting.Sort((a, b) => a.mValue.CompareTo(b.mValue));

            Stopwatch sw = new Stopwatch();
            sw.Start();
            while (sorting.Count != 0)
            {
                Move insp = sorting.Last();
                sorting.Remove(insp);
                board.move(insp.srce.X, insp.srce.Y, insp.dest.X, insp.dest.Y);
                insp.bValue = negaMaxAlphaBeta(
                    board,
                    depth,
                    int.MinValue,
                    int.MaxValue,
                    team,
                    sw);
                done.Add(insp);
                board.rollBackMove();
                if (sw.ElapsedMilliseconds > MAX_MS)
                {
                    Console.WriteLine("******OverTime!******* {0}", sw.ElapsedMilliseconds);
                    break;
                }
            }

            done.Sort((a, b) => a.bValue.CompareTo(b.bValue));
            return done.Last();
        }

        int negaMaxAlphaBeta(
            ChessBitBoard board,
            int depth,
            int alpha,
            int beta,
            Team team,
            Stopwatch sw)
        { //01 function negamax(node, depth, α, β, color)
            //04     childNodes := GenerateMoves(node)
            Queue<Move> moves = new Queue<Move>();
            board.getAllValidMoves((team == Team.White) ? Team.Black : Team.White, moves);

            //02     if depth = 0 or node is a terminal node
            //03         return color * the heuristic value of node
            if (sw.ElapsedMilliseconds > MAX_MS || depth == 0 || moves.Count == 0)
                return ((team == Team.White) ? 1 : -1) * negaHeuristic(board);

            //05     childNodes := OrderMoves(childNodes)
            List<Move> sorted = new List<Move>();
            while (moves.Count != 0)
            {
                moves.Peek().mValue = weighMove(team, board, moves.Peek());
                sorted.Add(moves.Dequeue());
            }
            sorted.Sort((a, b) => a.mValue.CompareTo(b.mValue));

            //06     bestValue := −∞
            int bestValue = int.MinValue;

            //07     foreach child in childNodes
            while (sorted.Count != 0)
            {
                // Make move here.
                Move insp = sorted.Last();
                sorted.Remove(insp);
                board.move(insp.srce.X, insp.srce.Y, insp.dest.X, insp.dest.Y);

                //08         v := −negamax(child, depth − 1, −β, −α, −color)
                int v = -negaMaxAlphaBeta(
                    board,
                    depth - 1,
                    -beta,
                    -alpha,
                    (team == Team.White) ? Team.Black : Team.White,
                    sw);

                // Roll back move here.
                board.rollBackMove();

                //Console.WriteLine ("V: {0} W: {1}", v, w);
                //09         bestValue := max( bestValue, v )
                bestValue = Math.Max(bestValue, (v));

                //10         α := max( α, v )
                alpha = Math.Max(alpha, (v));

                //11         if α ≥ β
                //12             break
                if (alpha >= beta) break;
            }

            //13     return bestValue
            return bestValue;
        }

        private static int negaHeuristic(ChessBitBoard board)
        {
            // Heuristic Value should be calculated of the state of the board.
            // A positive value means white-advantage, negative means black.
            const int pawn = 1;
            const int rook = 5;
            const int bishkni = 3;
            const int queen = 9;
            const int king = 200;
            const int mobilityW = 1;

            int wKings = ChessBitBoard.countBoardPop(board.getOccupancy(Team.White, Piece.King));
            int bKings = ChessBitBoard.countBoardPop(board.getOccupancy(Team.Black, Piece.King));

            int wQueens = ChessBitBoard.countBoardPop(board.getOccupancy(Team.White, Piece.Queen));
            int bQueens = ChessBitBoard.countBoardPop(board.getOccupancy(Team.Black, Piece.Queen));

            int wRooks = ChessBitBoard.countBoardPop(board.getOccupancy(Team.White, Piece.Rook));
            int bRooks = ChessBitBoard.countBoardPop(board.getOccupancy(Team.Black, Piece.Rook));

            int wBishops = ChessBitBoard.countBoardPop(board.getOccupancy(Team.White, Piece.Bishop));
            int bBishops = ChessBitBoard.countBoardPop(board.getOccupancy(Team.Black, Piece.Bishop));

            int wKnights = ChessBitBoard.countBoardPop(board.getOccupancy(Team.White, Piece.Knight));
            int bKnights = ChessBitBoard.countBoardPop(board.getOccupancy(Team.Black, Piece.Knight));

            int wPawns = ChessBitBoard.countBoardPop(board.getOccupancy(Team.White, Piece.Pawn));
            int bPawns = ChessBitBoard.countBoardPop(board.getOccupancy(Team.Black, Piece.Pawn));

            //int wDoubles = board.getPawnFiles(PawnFile.Double, Team.White);
            //int bDoubles = board.getPawnFiles(PawnFile.Double, Team.Black);

            //int wBlocked = board.getBlockedPawns(Team.White);
            //int bBlocked = board.getBlockedPawns(Team.Black);

            //int wIsolated = board.getIsolatedPawns(Team.White);
            //int bIsolated = board.getIsolatedPawns(Team.Black);

            Queue<Move> moves = new Queue<Move>();
            board.getAllValidMoves(Team.White, moves);
            int wMoves = moves.Count;
            moves.Clear();
            board.getAllValidMoves(Team.Black, moves);
            int bMoves = moves.Count;

            int material = king * (wKings - bKings) +
                queen * (wQueens - bQueens) +
                rook * (wRooks - bRooks) +
                bishkni * ((wBishops - bBishops) + (wKnights - bKnights)) +
                pawn * (wPawns - bPawns);
            int mobility = mobilityW * (wMoves - bMoves);

            return (material + mobility);
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
