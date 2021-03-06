﻿using System.Collections.Generic;
using System.Linq;
using UvsChess;
using System;

namespace GroupEight
{
    public class RandomAI : BaseAI, IChessAI
    {
        #region IChessAI Members that are implemented by the Student

        public RandomAI()
        {

        }

        /// <summary>
        /// The name of your AI
        /// </summary>
        public string Name
        {
#if DEBUG
            get { return "GroupEight_RandomAI (Debug)"; }
#else
            get { return "GroupEight_RandomAI"; }
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
            var allMyMoves = new Queue<Move>();
            var allEnemyMoves = new Queue<Move>();
            var ourTeam = myColor == ChessColor.Black ? Team.Black : Team.White;
            var enemyTeam = ourTeam == Team.White ? Team.Black : Team.White;

            bitBoard.getAllValidMoves(ourTeam, allMyMoves);

            if (IsStalemate(bitBoard, allMyMoves, enemyTeam))
            {
                return new ChessMove(new ChessLocation(0, 0), new ChessLocation(0, 0), ChessFlag.Stalemate);
            }

            var rng = new Random();
            var randomMove = allMyMoves.ElementAt(rng.Next(allMyMoves.Count));
            var flag = GetMoveFlag(bitBoard, randomMove, enemyTeam);

            // convert from our cartesian coordinates to the frameworks
            var srcy = 7 - randomMove.srce.Y;
            var desty = 7 - randomMove.dest.Y;

            return new ChessMove(new ChessLocation(randomMove.srce.X, srcy), new ChessLocation(randomMove.dest.X, desty), flag);
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
