using System;
using System.Collections.Generic;
using System.Text;
using UvsChess;

namespace GroupEight
{
    public partial class ChessBitBoard
    {
        /* Returns a string representation of the entire board (every piece masks OR'd together). */
        public override string ToString()
        {
            return BoardToString(getOccupancy(Team.Max, Piece.Max));
        }

        /** @brief Prints a String Representation of a UInt64
		 *
		 *  Given a UInt64, it will return a string of the bit board representation.
		 *
		 * @param board - UInt64 containing board to print.
		 * @return string containing textual representation of the board given.
		 */
        public static string BoardToString(UInt64 board)
		{
			UInt64 bitMask = 0x8000000000000000;
			StringBuilder sb = new StringBuilder();
			for (int bit = 0; bit < MAX_BITS; bit++)
			{
				if ((board & bitMask) > 0)
					sb.Append('1');
				else
					sb.Append('0');

				if ((bit + 1) % EIGHT_BITS == 0)
					sb.AppendLine();

				bitMask >>= 1;
			}

			return sb.ToString();
		}

        /** @brief Prints a String Representation of Queue of UInt64s
		 *
		 *  Given a Queue of UInt64s, it will return a string of the bit board
		 *  representations formatted for easy output
		 * 
		 *  YOU can change LINE_BREAK to change the number per row.
		 *
		 * @param inQueue a queue containing the UInt64s that need formatting
		 * @return string containing textual representation of the boards given.
		 */
        public static string BoardsToStringRows(Queue<UInt64> inQueue)
        {
            /* Nothing to do */
            if (inQueue.Count == 0)
                return "";
            else if (inQueue.Count == 1)
                return BoardToString(inQueue.Dequeue());

            /* Initialize */
            var sb = new StringBuilder("");
            var boardList = new List<string>();
            const int LINE_BREAK = 8;
            const int TOP = EIGHT_BITS - 1;

            /* Convert to string rep */
            while (inQueue.Count != 0)
                boardList.Add(BoardToString(inQueue.Dequeue()));

            int start = 0;
            int end = (boardList.Count >= LINE_BREAK) ? LINE_BREAK - 1 : boardList.Count - 1;
            while (start < end)
            {
                for (int rank = 0; rank <= TOP; ++rank)
                    for (int item = start; item <= end; ++item)
                    {
                        sb.Append(boardList[item].Substring(EIGHT_BITS * rank + 1 * rank, EIGHT_BITS));
                        if (item != end)
                            sb.Append(" ");
                        else
                            sb.Append("\n");
                    }

                sb.Append("\n");
                start += LINE_BREAK;
                end = ((end + LINE_BREAK) < boardList.Count) ? end + LINE_BREAK : boardList.Count - 1;
            }

            return sb.ToString();
        }

        /* To Output BitBoard Values for Debugging to Output to the Console */
        public void OutputBitBoardValues()
        {
            for (int t = 0; t < MAX_TEAMS; t++)
            {
                for (int p = 0; p < BIT_BOARDS_PER_TEAM; p++)
                {

                    var sb = new StringBuilder();

                    sb.Append((Team)t + " ");
                    sb.Append((Piece)p);

                    GuiLogLine(sb.ToString());
                    GuiLogLine("BitBoardValue");
                    GuiLogLine("Hex: {0,16:X16} Int: {0,30:N0}", BitBoardPieceMasks[t][p]);
                }

                GuiLogLine();
            }
        }

        /* Part of Debugging BitBoard Representation to Output the Bit Boards to the Console */
        public void OutPutBitBoardRepresentaion()
        {
            Stack<char>[][] bitBoardOutputStacks = new Stack<char>[MAX_TEAMS][];
            bitBoardOutputStacks[0] = new Stack<char>[BIT_BOARDS_PER_TEAM];
            bitBoardOutputStacks[1] = new Stack<char>[BIT_BOARDS_PER_TEAM];

            for (int t = 0; t < MAX_TEAMS; t++)
            {
                for (int p = 0; p < BIT_BOARDS_PER_TEAM; p++)
                {
                    UInt64 tempBitBoardValue = BitBoardPieceMasks[t][p];
                    UInt64 bitMask = 1;
                    bitBoardOutputStacks[t][p] = new Stack<char>();

                    for (int bit = 0; bit < MAX_BITS; bit++)
                    {
                        if ((tempBitBoardValue & bitMask) > 0)
                        {
                            bitBoardOutputStacks[t][p].Push('1');
                        }
                        else
                        {
                            bitBoardOutputStacks[t][p].Push('0');
                        }
                        bitMask = bitMask << 1;
                    } /* End MAX_BITS for Loop */
                } /* End Piece For Loop */
            } /* End Team For Loop */

            for (int t = 0; t < MAX_TEAMS; t++)
            {
                int pieceCnt = 0;
                var sb = new StringBuilder();

                GuiLogLine("Team: {0}", ((Team)t).ToString());
                GuiLogLine("{0,-9}{1,-9}{2,-9}{3,-9}{4,-9}{5,-9}", Piece.King, Piece.Queen, Piece.Bishop, Piece.Knight, Piece.Rook, Piece.Pawn);

                while (bitBoardOutputStacks[t][pieceCnt].Count != 0)
                {
                    int bitCnt = 0;

                    while (bitCnt < EIGHT_BITS)
                    {
                        sb.Append(bitBoardOutputStacks[t][pieceCnt].Peek());
                        bitBoardOutputStacks[t][pieceCnt].Pop();
                        bitCnt++;
                    }

                    sb.Append(" ");
                    pieceCnt++;

                    if (pieceCnt == BIT_BOARDS_PER_TEAM)
                    {
                        GuiLogLine(sb.ToString());
                        sb = new StringBuilder();
                        pieceCnt = 0;
                    }
                }

                GuiLogLine();
            }

            GuiLogLine();
        }

        /* Helper Function for OutPutPieceMasks() to output 16 bit masks in a row */
        private void OutPutPieceMasksHelper(string team, int piece, string boardType, Stack<char>[] bitBoardStacks)
        {
            int curPosition = MAX_BITS - 1;
            int curPositionMod = (curPosition % (EIGHT_BITS * 2));

            GuiLogLine("Team: {0:N0} Piece: {1:N0} Mask: {2:N0}", team, ((Piece)piece).ToString(), boardType);
            //LogLine("Cur Position: {0:N0} Cur Pos Mod: {1:N0} Stack Count: {2:N0}", curPosition, curPositionMod, bitBoardStacks[curPositionMod].Count());

            while (bitBoardStacks[curPositionMod].Count != 0 && curPosition > -1)
            {
                int incCnt = 0;
                GuiLogLine("{0,-9:N0}{1,-9:N0}{2,-9:N0}{3,-9:N0}{4,-9:N0}{5,-9:N0}{6,-9:N0}{7,-9:N0}{8,-9:N0}{9,-9:N0}{10,-9:N0}{11,-9:N0}{12,-9:N0}{13,-9:N0}{14,-9:N0}{15,-9:N0}", curPosition - incCnt++, // 0
                    curPosition - incCnt++, // 1
                    curPosition - incCnt++, // 2
                    curPosition - incCnt++, // 3
                    curPosition - incCnt++, // 4
                    curPosition - incCnt++, // 5
                    curPosition - incCnt++, // 6
                    curPosition - incCnt++, // 7
                    curPosition - incCnt++, // 8
                    curPosition - incCnt++, // 9
                    curPosition - incCnt++, // 10
                    curPosition - incCnt++, // 11
                    curPosition - incCnt++, // 12
                    curPosition - incCnt++, // 13
                    curPosition - incCnt++, // 14
                    curPosition - incCnt++ // 15
                );

                int rankCnt = 0;
                int curPosStart = curPosition;
                var sb = new StringBuilder();

                while (rankCnt < EIGHT_BITS)
                {
                    int bitCnt = 0;
                    while (bitCnt < EIGHT_BITS)
                    {
                        int posMod = curPosition % (EIGHT_BITS * 2);
                        sb.Append(bitBoardStacks[posMod].Peek());
                        bitBoardStacks[posMod].Pop();
                        bitCnt++;
                    }

                    sb.Append(" ");
                    curPosition--;

                    if ((curPosition + 1) % (EIGHT_BITS * 2) == 0)
                    {
                        GuiLogLine(sb.ToString());
                        sb = new StringBuilder();

                        rankCnt++;
                        curPosition = curPosStart;
                    }
                } /* End While (rankCnt < EIGHT_BITS) */

                curPosition -= EIGHT_BITS * 2;
                if (curPosition < 0)
                {
                    curPosition = MAX_BITS - 1;
                }
                curPositionMod = (curPosition % (EIGHT_BITS * 2));

                GuiLogLine();
            } /* End While (!Stack.empty()) */
        }

        public void OutputPieceHexMasksHelper()
        {
            for (int i = 0; i < BIT_BOARDS_PER_TEAM; i++)
            {
                if (i != (int) Piece.Pawn)
                {
                    GuiLogLine("Piece: {0}",((Piece)i).ToString());
                    for (int j = 0; j < MAX_BITS; j++)
                    {
                        GuiLogLine("0x{0,16:X16},", AllButPawnMoveMasks[i][j]);
                    }
                }
                else
                {
                    for (int theTeam = 0; theTeam < MAX_TEAMS; theTeam++)
                    {
                        for (int theMask = 0; theMask < MAX_TEAMS; theMask++)
                        {
                            if (theMask == 0)
                            {
                                GuiLogLine("Team: {0} Piece: {1} Mask: Move", ((Team)theTeam).ToString(), ((Piece)i).ToString());
                                for (int theBit = 0; theBit < MAX_BITS; theBit++)
                                {
                                    GuiLogLine("0x{0,16:X16},", PawnMoveMasks[theTeam][theBit]);
                                }
                            }
                            else
                            {
                                GuiLogLine("Team: {0} Piece: {1} Mask: Capture", ((Team)theTeam).ToString(), ((Piece)i).ToString());
                                for (int theBit = 0; theBit < MAX_BITS; theBit++)
                                {
                                    GuiLogLine("0x{0,16:X16},", PawnCaptureMasks[theTeam][theBit]);
                                }
                            }
                        }
                    }
                }
            }
        }

        /* Output for Debugging Piece Move Masks to Output to the Console */
        public void OutPutPieceMasks()
        {
            Stack<char>[] bitBoardOutputStacks = new Stack<char>[EIGHT_BITS * 2];

            for (int x = 0; x < (EIGHT_BITS * 2); x++)
                bitBoardOutputStacks[x] = new Stack<char>();
            /* Loops through the 6 different type of pieces */
            for (int p = 0; p < BIT_BOARDS_PER_TEAM; p++)
            {
                int tempPawnTeam;
                string curTeam = "Either";
                string bitBoardMask = "Move and Capture";
                /* Checks if current Piece type is a Pawn to set tempPawnTeam to 
                 * loop through the black and white Pawn Move and Capture Masks */
                tempPawnTeam = p == (BIT_BOARDS_PER_TEAM - 1) ? MAX_TEAMS : 1;
                /* Will Loop twice for a pawn */
                for (int pawnTeam = 0; pawnTeam < tempPawnTeam; pawnTeam++)
                {
                    /* If current Piece is a Pawn, set the teams */
                    if (tempPawnTeam == MAX_TEAMS)
                        curTeam = pawnTeam == (int)Team.White ? "White" : "Black";

                    /* Will loop twice When Current Piece is a pawn to get the Move and Capture masks */
                    for (int movCapMask = 0; movCapMask < tempPawnTeam; movCapMask++)
                    {
                        /* Loops for each Square position of a Move or Capture Mask  */
                        for (int curBitPositionMask = 0; curBitPositionMask < MAX_BITS; curBitPositionMask++)
                        {
                            UInt64 tempBitBoardValue;
                            /* If current piece is a pawn set the bitBoardMask to either Moves or Capture */
                            if (p == (BIT_BOARDS_PER_TEAM - 1))
                            {
                                if (movCapMask == 0)
                                {
                                    tempBitBoardValue = PawnMoveMasks[pawnTeam][curBitPositionMask];
                                    bitBoardMask = "Moves";
                                }
                                else
                                {
                                    tempBitBoardValue = PawnCaptureMasks[pawnTeam][curBitPositionMask];
                                    bitBoardMask = "Capture";
                                }
                            }
                            else
                                /* For All other Pieces */
                                tempBitBoardValue = AllButPawnMoveMasks[p][curBitPositionMask];

                            UInt64 bitMask = 1;
                            /* Loops through each bit of a mask */
                            for (int bit = 0; bit < MAX_BITS; bit++)
                            {
                                if ((tempBitBoardValue & bitMask) > 0)
                                    bitBoardOutputStacks[curBitPositionMask % (EIGHT_BITS * 2)].Push('1');
                                else
                                    bitBoardOutputStacks[curBitPositionMask % (EIGHT_BITS * 2)].Push('0');
                                bitMask = bitMask << 1;
                            } /* End curBitPosition Mask Bit For Loop */
                        } /* End Piece curBitPosition Mask For Loop */

                        /* Only For The Pawn Piece Run */
                        if (p == (BIT_BOARDS_PER_TEAM - 1))
                            OutPutPieceMasksHelper(curTeam, p, bitBoardMask, bitBoardOutputStacks);
                    }
                }
                /* All other Pieces */
                if (p != (BIT_BOARDS_PER_TEAM - 1))
                    OutPutPieceMasksHelper(curTeam, p, bitBoardMask, bitBoardOutputStacks);
                GuiLogLine();
            } /* End Pieces For Loop */
        }
    }
}