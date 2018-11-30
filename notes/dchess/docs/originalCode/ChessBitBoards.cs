
public enum PawnFile { Double = 2, Triple = 3 , Quadruple = 4 };

public class Move {
  public Coordinate srce;
  public Coordinate dest;
  public Piece capture;
  public bool causesEnemyThreat;
  public bool srcUnderThreat;
  public bool destUnderThreat;
  public bool causesCheck;
  public bool pawnToQueen;
  public int mValue;
  public int bValue;

  public int value { get { return mValue + bValue; } }

  int _score = -1;
  public int Score(Move move, ChessBitBoard board, Team team) {
    if (_score == -1) {
      _score = Evaluate(move, board, team);
    }
    return _score;
  }

  public Func<Move, ChessBitBoard, Team, int> Evaluate;

  public override string ToString() {
    StringBuilder sb = new StringBuilder();
    sb.Append ("SrcX/Y:" + srce.X + "/" + srce.Y + " ");
    sb.Append ("DstX/Y:" + dest.X + "/" + dest.Y + " ");
    sb.Append ("cap:" + capture + " ");
    sb.Append ("CET:" + causesEnemyThreat + " ");
    sb.Append ("SUT:" + srcUnderThreat + " ");
    sb.Append ("DUT:" + destUnderThreat + " ");
    sb.Append ("CC:" + causesCheck + " ");
    sb.Append ("PtQ:" + pawnToQueen + " ");
    sb.Append ("mVal:" + mValue + " ");
    sb.Append ("bVal:" + bValue);
    return sb.ToString();
  }
};

Stack<Move> rollBackStack = new Stack<Move> ();

public enum Direction {
  left = 0, upLeft, up, upRight, right, downRight, down, downLeft };

/** @brief Retrieves Blocked Pawn Count
  *
  * This function will give you the number of double, triple or
  * quadruple occurances of pawn filess.
  *
  * @param file Degree of file to be counted.
  * @param team Team to count
  */
public int getPawnFiles(PawnFile file, Team team) {
  //doubled, - two pawns on the same file. (also triple and quadruple)
  int output = 0;
  UInt64 pawns = BitBoardPieceMasks [(int)team] [(int)Piece.Pawn];
  Queue<Coordinate> coords = new Queue<Coordinate> (64);
  boardToIndexes (pawns, coords);
  int[] counts = new int[EIGHT_BITS];

  while (coords.Count != 0) {
    Coordinate pwn = coords.Dequeue ();
    counts [pwn.X]++;
  }

  for (int i = 0; i < EIGHT_BITS; ++i) {
    switch (file) {
      case PawnFile.Double: {
        if (counts [i] == 2)
        output++;
        break;
      }
      case PawnFile.Triple: {
        if (counts [i] == 3)
        output++;
        break;
      }
      case PawnFile.Quadruple: {
        if (counts [i] == 4)
        output++;
        break;
      }
    }
  }

  return output;
}

/** @brief Retrieves pawns on rank
  *
  * This function will return how many pawns are in the same rank
  *
  * @param team Team to count
  * @param rank Degree of rank to be counted.
  */
public int pawnCountForRank(Team team, int rank) {
  int output = 0;
  UInt64 pawns = BitBoardPieceMasks[(int)team][(int)Piece.Pawn];
  Queue<Coordinate> coords = new Queue<Coordinate>(64);
  boardToIndexes(pawns, coords);

  while (coords.Count != 0) {
    Coordinate pwn = coords.Dequeue();
    if (pwn.Y == rank) {
      output++;
    }
  }

  return output;
}

/** @brief Retrieves Blocked Pawn Count
  *
  * This function, given a team will tell you the count of how many
  * pawns that team currently has blocked.
  *
  * @param team Team to count
  * @return int with count.
  */
public int getBlockedPawns(Team team) {
  //blocked - a pawn that cannot advance
  int output = 0;
  UInt64 pawns = BitBoardPieceMasks [(int)team] [(int)Piece.Pawn];
  Queue<Coordinate> coords = new Queue<Coordinate> (64);
  boardToIndexes (pawns, coords);
  while (coords.Count != 0) {
    Coordinate pwn = coords.Dequeue ();
    if (getMoves (pwn.X, pwn.Y) == 0)
      output++;
  }
  return output;
}

/** @brief Retrieves Isolated Pawn Count
  *
  * This function, given a team will tell you the count of how many
  * pawns are isolated.
  *
  * @param team Team to count
  * @return int with count.
  */
public int getIsolatedPawns(Team team) {
  //isolated pawns - a pawn with no friendly pawns on the adjacent files
  int output = 0;
  UInt64 pawns = BitBoardPieceMasks [(int)team] [(int)Piece.Pawn];
  Queue<Coordinate> coords = new Queue<Coordinate> (64);
  boardToIndexes (pawns, coords);
  int[] counts = new int[EIGHT_BITS];
  while (coords.Count != 0) {
    Coordinate pwn = coords.Dequeue ();
    counts [pwn.X]++;
  }
  for (int i = 0; i < EIGHT_BITS; ++i) {
    if (i == 0) {
      if (counts [i] == 1 && counts [i + 1] == 0) {
        output++;
      }
    } else if (i == 7) {
      if (counts [i] == 1 && counts [i - 1] == 0) {
        output++;
      }
    } else
    if (counts [i - 1] == 0 && counts [i] == 1 && counts [i + 1] == 0)
      output++;
  }

  return output;
}

/** @brief Generates Possible Moves
  *
  *	Given a location will generate a map of all possible moves for
  *  the piece at that location. If there is no piece at the location
  *  an empty map will be returned;
  *
  *  This function has **NO** bounds checking
  *
  * @param x/y coordinate
  * @return UInt64 containing the map of possible moves.
  */
public UInt64 getMoves(int x, int y) {
  UInt64 output = 0;
  const int TOP = EIGHT_BITS - 1;
  Team team = getPieceTeam (x, y);
  if (team == Team.Error) return output;
  Piece piece = getPieceType (x, y);
  UInt64 occupied = getOccupancy (Team.Max, Piece.Max);
  int s = (y * EIGHT_BITS) + (TOP - x);

  switch (piece) {
    case Piece.King:
    case Piece.Knight: {
      UInt64 moveMask = AllButPawnMoveMasks[(int)piece][s];
      output = moveMask & ~occupied;
      break;
    }
    case Piece.Bishop: {
      output =
        BISHOP_ATTACKS[s, magicIndexBishop(s, occupied)] & ~occupied;
      break;
    }
    case Piece.Rook: {
      output = ROOK_ATTACKS[s, magicIndexRook(s, occupied)] & ~occupied;
      break;
    }
    case Piece.Queen: {
      output =
        (BISHOP_ATTACKS[s, magicIndexBishop(s, occupied)] |
        ROOK_ATTACKS[s, magicIndexRook(s, occupied)]) &
        ~occupied;
      break;

      //UInt64 moveMask = AllButPawnMoveMasks[(int)piece][s];
      //UInt64 oldOutput =
      //  moveMask ^ getRayMask(x, y, (moveMask & occupied), true);
    }
    case Piece.Pawn: {
      UInt64 moveMask = PawnMoveMasks[(int)team][s];
      if( (y == 1 && team == Team.White &&
            (getPieceType (x, y + 1) != Piece.Error)) ||
          (y == 6 && team == Team.Black &&
            (getPieceType (x, y - 1) != Piece.Error)))
        break;
      else output = moveMask & ~occupied;
      break;
    }
    case Piece.Error:
    case Piece.Max: {
      Console.WriteLine ("This should never happen");
      break;
    }
  }
  return output;
}

/** @brief Generate Attacks
  *
  *	Given a location will generate a map of possible attacks for
  *  the piece in that location. If there is no piece in the
  *  location given, the attack map should come back blank.
  *
  *  This function has **NO* bounds checking
  *
  * @param x/y coordinate for generations
  * @return UInt64 containing
  */
public UInt64 getAttacks(int x, int y) {
  UInt64 output = 0;
  const int TOP = EIGHT_BITS - 1;
  Team team = getPieceTeam (x, y);
  if (team == Team.Error) return output;
  Piece piece = getPieceType (x, y);
  UInt64 enemies =
    getOccupancy(
      ((team == Team.White) ? Team.Black : Team.White), Piece.Max);
  int s = (y * EIGHT_BITS) + (TOP - x);

  switch (piece) {
    case Piece.King:
    case Piece.Knight: {
      UInt64 moveMask = AllButPawnMoveMasks[(int)piece][s];
      output = moveMask & enemies;
      break;
    }
    case Piece.Bishop: {
      UInt64 blockers = getOccupancy(Team.Max, Piece.Max);
      output =
        BISHOP_ATTACKS[s, magicIndexBishop(s, blockers)] & enemies;
      break;
    }
    case Piece.Rook: {
      UInt64 blockers = getOccupancy(Team.Max, Piece.Max);
      output = ROOK_ATTACKS[s, magicIndexRook(s, blockers)] & enemies;
      break;
    }
    case Piece.Queen: {
      UInt64 blockers = getOccupancy(Team.Max, Piece.Max);
      output =
        (BISHOP_ATTACKS[s, magicIndexBishop(s, blockers)] |
        ROOK_ATTACKS[s, magicIndexRook(s, blockers)]) & enemies;
      break;

      //UInt64 moveMask = AllButPawnMoveMasks[(int)piece][s];
      //output = (moveMask ^ getRayMask(x, y, (moveMask & blockers), false)) & enemies;
    }
    case Piece.Pawn: {
      UInt64 moveMask = PawnCaptureMasks[(int)team][s];
      output = moveMask & enemies;
      break;
    }
    case Piece.Error:
    case Piece.Max: {
      Console.WriteLine ("This should never happen");
      break;
    }
  }
  return output;
}

/** @brief Checks Check Conditions for Specific Team
  *
  *	Given a Team this function will return a map that has all pieces that
  *  currently threaten that team's King
  *
  * @param team Team to check for check condition
  * @param UInt64 containing all possible king threats
  */
public UInt64 checkCheck(Team team) {
  #if BoundsCheck
  if (team == Team.Error || team == Team.Max) {
    Console.Error.WriteLine("checkCheck: bounds checking");
    return 0;
  }
  #endif

  UInt64 output = 0;
  UInt64 kingBoard = BitBoardPieceMasks[(int)team][(int)Piece.King];
  Queue<Coordinate> indexes = new Queue<Coordinate>(64);
  boardToIndexes (kingBoard, indexes);
  while (indexes.Count > 0) {
    output |= threatCheck (indexes.Peek ().X, indexes.Peek ().Y, team);
    indexes.Dequeue ();
  }

  return output;
}

/** @brief Gets All Available Moves
  *
  *	Given a team, this function will return all valid moves for that given team
  *
  *	@param team Team to give valid moves for.
  *  @param moves EMTPY list to store moves in.
  */
public void getAllValidMoves(Team team, Queue<Move> moves) {
  for (int i = 0; i < SIX_BITS; ++i) {
    Queue<Coordinate> src = new Queue<Coordinate> (64);
    boardToIndexes (BitBoardPieceMasks [(int)team] [i], src);

    while (src.Count != 0) {
      Coordinate sc = src.Dequeue ();
      Queue<Coordinate> dest = new Queue<Coordinate> (64);
      boardToIndexes(getMoves(sc.X, sc.Y), dest);
      boardToIndexes(getAttacks(sc.X, sc.Y), dest);

      while (dest.Count != 0) {
        Coordinate ds = dest.Dequeue ();
        if (validateMove (sc.X, sc.Y, ds.X, ds.Y)) {
          move (sc.X, sc.Y, ds.X, ds.Y);
          moves.Enqueue (lastMove ());
          rollBackMove ();
        }
      }
    }
  }
}

/** @brief Moves a Piece from One Location to Another.
  *
  * This function will indiscriminately move a peice from the source square
  * to the destination square. It will remove any piece in the destination
  * square, and put the move on the move stack so it can be reverted.
  *
  * given an invalid source or destination, this function will do nothing.
  * Also moving to the same location is considered an invalid move and won't
  * be recorded on the roll-back.
  *
  *  This function has bounds checking
  *
  * @param sx/sy/dx/dy Ints with source and destination x/y
  */
public void move(int sx, int sy, int dx, int dy) {
  #if BoundsCheck
    const int TOP = EIGHT_BITS - 1;
    if (sx < 0 || sx > TOP || sy < 0 || sy > TOP ||
      dx < 0 || dx > TOP || dy < 0 || dy > TOP ||
      ((sx == dx) && sy == dy)) {
      Console.Error.WriteLine("move: bounds checking");
      return;
    }
  #endif

  Move move = new Move();

  /* Set Source and Dest */
  move.srce.X = sx;
  move.srce.Y = sy;
  move.dest.X = dx;
  move.dest.Y = dy;

  /* Determine Capture */
  Team team = getPieceTeam (sx, sy);
  Team enemy = getPieceTeam (dx, dy);
  if (team != enemy && enemy != Team.Error )
  move.capture = getPieceType(dx, dy);
  else
  move.capture = Piece.Error;

  /* Currently Under Threat? */
  move.srcUnderThreat = threatCheck (sx, sy, team) != 0 ;

  /* Pawn to Queen? */
  move.pawnToQueen = false;
  Piece type = getPieceType (sx, sy);
  if (type == Piece.Pawn)
    if (team == Team.Black) {
      if (dy == 0) {
        setPiece(team, Piece.Queen, dx, dy);
        move.pawnToQueen = true;
      } else setPiece(team, type, dx, dy);
    } else {
      if (dy == 7) {
        setPiece(team, Piece.Queen, dx, dy);
        move.pawnToQueen = true;
      } else setPiece(team, type, dx, dy);
    } else setPiece(team, type, dx, dy);

  /* Remove piece from source */
  unSetPiece(sx, sy);

  /* Does this move cause check? */
  move.causesCheck = false;
  if(checkCheck((team == Team.Black) ? Team.White : Team.Black) != 0)
  move.causesCheck = true;

  move.causesEnemyThreat = false;
  if(getAttacks(dx, dy) != 0)
  move.causesEnemyThreat = true;

  move.destUnderThreat = false;
  if(threatCheck(dx, dy, team) != 0)
  move.destUnderThreat = true;

  rollBackStack.Push(move);
}

/** @brief Rolls Back a Single Move
  *
  * This function will roll back the board by one move.
  *
  * @param nMoves Integer with the number of
  * @return Move move that was rolled back.
  */
public void rollBackMove() {
  Move move = rollBackStack.Peek();
  Team team = getPieceTeam(move.dest.X, move.dest.Y);
  Piece piece = getPieceType(move.dest.X, move.dest.Y);

  if (move.pawnToQueen)
    setPiece (team, Piece.Pawn, move.srce.X, move.srce.Y);
  else
    setPiece (team, piece, move.srce.X, move.srce.Y);

  unSetPiece (move.dest.X, move.dest.Y);

  if (move.capture != Piece.Error)
    setPiece (
      (team == Team.Black) ? Team.White : Team.Black,
      move.capture,
      move.dest.X,
      move.dest.Y);

  rollBackStack.Pop ();
  return;
}

/** @brief Rolls Back Moves
  *
  * This function will, given an integer, roll back the board the
  * number of moves given. If the integer is out of range, it will
  * roll back *ALL* moves on the board.
  *
  * (ex: -1 or > Count will roll back all moves.)
  *
  * This function has bound checking.
  *
  * @param nMoves Integer with the number of
  */
public void rollBackMoves(int nMoves) {
  int rollbacks =
  (nMoves < 1 || nMoves > rollBackStack.Count) ?
    rollBackStack.Count : nMoves ;

  for (int i = 1; i <= rollbacks; ++i)
  rollBackMove ();

  return;
}

/** @brief Returns Last Move
  *
  * This function will return the last move made on the board
  *
  * this function has *NO* bounds checking
  *
  * @return Move with last move
  */
public Move lastMove() { return rollBackStack.Peek (); }

/** @brief Rolls Back Moves
  *
  * This function will clear the rollback stack
  *
  */
public void clearRollBack() { rollBackStack.Clear (); }

/** @brief Number of Moves on Rollback Stack
  *
  * This function will return the number of moves currently on the
  * rollback queue
  *
  * @return Int with # of moves currently on rollback
  */
public int movesOnStack() { return rollBackStack.Count; }

/** @brief Determines if a square is under threat
  *
  *	This function, given a location on the board will return a map of
  *  all pieces that currently threaten it. The spot on the board need
  *  not be occupied.
  *
  * @param x/y square to check for threat.
  * @param team Team to check if they are the ones threatened in said square
  * @return UInt64 containing all pieces threating said square.
  */
public UInt64 threatCheck(int x, int y, Team team) {
  const int TOP = EIGHT_BITS - 1;
  UInt64 output = 0;
  UInt64 enemies =
    getOccupancy(
      ((team == Team.White) ? Team.Black : Team.White),
      Piece.Max);
  UInt64 blockers = getOccupancy (Team.Max, Piece.Max);
  Queue<Coordinate> coords = new Queue<Coordinate>(64);
  int s = (y * EIGHT_BITS) + (TOP - x);

  UInt64 knightEnemies =
    AllButPawnMoveMasks [(int)Piece.Knight] [s] & enemies;
  coords.Clear ();
  boardToIndexes (knightEnemies, coords);
  while (coords.Count != 0) {
    Coordinate pe = coords.Dequeue ();
    if (getPieceType (pe.X, pe.Y) == Piece.Knight)
      output |= (FILE_MASKS[pe.X] & RANK_MASKS[pe.Y]);
  }

  //UInt64 rookMask =
  //	AllButPawnMoveMasks [(int)Piece.Rook] [s];
  //UInt64 rookEnemies =
  //	(rookMask ^ getRayMask(x, y, (rookMask & blockers), false)) & enemies;
  UInt64 rookEnemies =
    ROOK_ATTACKS[s, magicIndexRook(s, blockers)] & enemies;
  coords.Clear ();
  boardToIndexes (rookEnemies, coords);
  while (coords.Count != 0) {
    Coordinate pe = coords.Dequeue ();
    Piece type = getPieceType (pe.X, pe.Y);
    if (type == Piece.Rook || type == Piece.Queen)
      output |= (FILE_MASKS[pe.X] & RANK_MASKS[pe.Y]);
  }

  //UInt64 bishopMask =
  //	AllButPawnMoveMasks [(int)Piece.Bishop] [s];
  //UInt64 bishopEnemies =
  //	(bishopMask ^ getRayMask(x, y, (bishopMask & blockers), false)) & enemies;
  UInt64 bishopEnemies =
    BISHOP_ATTACKS[s, magicIndexBishop(s, blockers)] & enemies;
  coords.Clear ();
  boardToIndexes (bishopEnemies, coords);
  while (coords.Count != 0) {
    Coordinate pe = coords.Dequeue ();
    Piece type = getPieceType (pe.X, pe.Y);
    if (type == Piece.Bishop || type == Piece.Queen)
      output |= (FILE_MASKS[pe.X] & RANK_MASKS[pe.Y]);
  }

  UInt64 kingMask =
  AllButPawnMoveMasks [(int)Piece.King] [(y * EIGHT_BITS) + (TOP - x)];
  UInt64 kingEnemies = kingMask & enemies;
  coords.Clear ();
  boardToIndexes (kingEnemies, coords);
  while (coords.Count != 0) {
    Coordinate pe = coords.Dequeue ();
    Piece type = getPieceType (pe.X, pe.Y);
    if (type == Piece.King)
      output |= (FILE_MASKS[pe.X] & RANK_MASKS[pe.Y]);
    if(type == Piece.Pawn && (pe.X - x) != 0) {
      if (((pe.Y - y) == 1 && team == Team.White) ||
        ((pe.Y - y) == -1 && team == Team.Black)) {
        output |= (FILE_MASKS[pe.X] & RANK_MASKS[pe.Y]);
      }
    }
  }
  return output;
}

/** @brief Determines if a Move is Valid
  *
  *	This function, given a source and destination, and assuming the source
  *  location is occupied, will return whether the move is valid (true) or
  *  invalid(false). It will return false if the piece does not exist.
  *  (as that is an invalid move)
  *
  *  Indexes out of range will return an invalid move.
  *
  *  This function has bounds checking
  *
  * @param sx/sy/dx/dy Ints with source and destination x/y
  * @return bool containing result.
  */
public bool validateMove(int sx, int sy, int dx, int dy) {
  #if BoundsCheck
  const int TOP = EIGHT_BITS - 1;
  if (sx < 0 || sx > TOP || sy < 0 || sy > TOP ||
    dx < 0 || dx > TOP || dy < 0 || dy > TOP ||
    ((sx == dx) && sy == dy)) {
    Console.Error.WriteLine("validateMove: bounds checking");
    return false;
  }
  #endif

  UInt64 valid =
  FILE_MASKS [dx] & RANK_MASKS [dy] &
    (getMoves (sx, sy) | getAttacks (sx, sy));

  Team team = getPieceTeam(sx, sy);
  move(sx, sy, dx, dy);
  UInt64 checkNext = checkCheck(team);
  rollBackMove();

  if(checkNext != 0) valid = 0;

  if(valid != 0)
    return true;
  else
    return false;
}

/** @brief Creates a Directional Ray from a Given Point
  *
  * This function, given a source and a direction will return a
  *  UInt64 that will contain a ray thrown from that point in the
  *  direction specified.
  *  Integers out of range will return a blank board.
  *
  * @param x/y Ints containing starting coords
  * @param d Direction of ray from point given
  * @return UInt64 containing board with ray cast.
  */
private static UInt64 castRay(int x, int y, Direction d) {
  UInt64 output = 0;
  const int TOP = EIGHT_BITS -1;
  #if BoundsCheck
  const int BOT = 0;
  if (x < BOT || x > TOP || y < BOT || y > TOP) {
    Console.Error.WriteLine("castRay: Bounds Checking x/y: {0}/{1}", x, y);
    return output;
  }
  #endif
  int shift;

  switch (d) {
    case Direction.up: {
      shift = EIGHT_BITS * y;
      output = (FILE_MASKS[x] >> shift) << shift;
      break;
    }
    case Direction.upRight : {
      shift = EIGHT_BITS * y;
      output = (DIAGONAL_MASKS_H8toA1[(TOP - x) + y] >> shift << shift);
      break;
    }
    case Direction.right : {
      shift = EIGHT_BITS * (TOP - y);
      output = ((RANK_MASKS [TOP] << x)  >> x) >> shift;
      break;
    }
    case Direction.downRight : {
      shift = EIGHT_BITS * (TOP - y);
      output = (DIAGONAL_MASKS_A8toH1[x+y] << shift ) >> shift;
      break;
    }
    case Direction.down: {
      shift = EIGHT_BITS * (TOP - y);
      output = (FILE_MASKS [x] << shift) >> shift;
      break;
    }
    case Direction.downLeft: {
      shift = EIGHT_BITS * (TOP - y);
      output = (DIAGONAL_MASKS_H8toA1 [(TOP - x) + y] << shift) >> shift;
      break;
    }
    case Direction.left : {
      shift = EIGHT_BITS * y;
      output = ((RANK_MASKS [0] >> (TOP - x)) << (TOP - x)) << shift;
      break;
    }
    case Direction.upLeft : {
      shift = EIGHT_BITS * y;
      output = (DIAGONAL_MASKS_A8toH1 [x + y] >> shift) << shift;
      break;
    }
  }
  return output;
}

/*TODO Document */
public static bool checkBounds(Coordinate xy) {
  if (xy.X < 0 || xy.Y < 0 || xy.X > 7 || xy.Y > 7)
    return false;
  return true;
}

/** @brief Generates Blocked Paths
  *
  *	This function, given a source and a blocker map (a map of any units
  *  that can prohibit movement) it will return a mask to use to determine
  *  possible, unblocked move for slider pieces.
  *  fromPoint will determine if the ray is cast FROM the point, or
  *  AFTER the point.
  *  Using an x/y that doesn't originate at the point the blocker map
  *  was generated from is undefined.
  *
  * @param x/y Ints with coordinates of the location of slider
  * @param blockers a map with blocking pieces generated for given loc.
  * @param fromPoint a bool containing whether you want the ray from point
  *   or after point
  * @return UInt64 containing blocked path squares.
  */
private static UInt64 getRayMask(int x, int y, UInt64 blockers, bool fromPoint) {
  UInt64 output = 0;
  const int TOP = EIGHT_BITS-1;
  const int BOT = 0;

  for (int i = 0; i <= (int)Direction.downLeft; ++i) {
    Direction dir = (Direction)i;
    int shift = getShift (dir);
    UInt64 mask = FILE_MASKS [x] & RANK_MASKS [y];
    Coordinate xy;
    xy.X = x;
    xy.Y = y;
    while(true) {
      xy = incrementXY(i, xy);
      if(shift > 0)
        mask <<= shift;
      else
        mask >>= Math.Abs(shift);
      if(xy.X < BOT || xy.X > TOP || xy.Y < BOT || xy.Y > TOP)
        break;

      if((mask & blockers) != 0) {
        if(!fromPoint)
          xy = incrementXY(i, xy);
        if(checkBounds(xy))
          output |= castRay(xy.X, xy.Y, dir);
        break;
      }
    }
  }
  return output;
}

/** @brief Returns Shift Number for Related Direction
  *
  *	This function, given a direction will return the number of shifts to
  *  move the given point one space in that direction.
  *  Remember if the number is negative, you will have to use the absolute
  *  value in a DOWN shift, as the negative won't work in an UP shift.
  *
  * @param d Direction of shift
  * @return integer with shift amount.
  */
private static int getShift(Direction d) {
  switch (d) {
    case Direction.left :
      return 1;
    case Direction.upLeft :
      return EIGHT_BITS+1;
    case Direction.up:
      return EIGHT_BITS;
    case Direction.upRight :
      return EIGHT_BITS-1;
    case Direction.right :
      return -1;
    case Direction.downRight :
      return -EIGHT_BITS - 1;
    case Direction.down:
      return -EIGHT_BITS;
    case Direction.downLeft:
      return -EIGHT_BITS + 1;
  }
  return 0;
}

private static UInt64 magicIndexRook(int s, UInt64 occupied) {
  return ((occupied & ROOK_MASKS[s]) * ROOK_MAGICS[s]) >>
    (int)ROOK_SHIFTS[s];
}

private static UInt64 magicIndexBishop(int s, UInt64 occupied) {
  return ((occupied & BISHOP_MASKS[s]) * BISHOP_MAGICS[s]) >>
    (int)BISHOP_SHIFTS[s];
}

