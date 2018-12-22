#ifndef CHESSGAME_HPP
#define CHESSGAME_HPP

#include <ostream>

class ChessGame {
public:

  /* Ctor/Xtor */
  ChessGame();

  ChessGame(const ChessGame& p_ot);

  ChessGame(ChessGame&& p_ot);

  ~ChessGame();

  /* Operator Overloads */
  ChessGame& operator=(const ChessGame& p_ot);

  ChessGame& operator=(ChessGame&& p_ot);

private:

  friend std::ostream& operator<<(std::ostream& p_os, const ChessGame& p_ot) {
    p_os << typeid(p_ot).name();
    return p_os;
  }
  friend std::wostream& operator<<(std::wostream& p_os, const ChessGame& p_ot) {
    p_os << typeid(p_ot).name();
    return p_os;
  }
};

#endif // CHESSGAME_HPP
