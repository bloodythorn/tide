#include "tide/games/chess/chessgame.hpp"

ChessGame::ChessGame() { }

ChessGame::ChessGame(const ChessGame& p_ot) { }

ChessGame::ChessGame(ChessGame&& p_ot) { }

ChessGame::~ChessGame() { }

ChessGame& ChessGame::operator=(const ChessGame& p_ot) {
  if(this != &p_ot) {};
  return *this;
}

ChessGame& ChessGame::operator=(ChessGame&& p_ot) {
  if(this != &p_ot) {};
  return *this;
}
