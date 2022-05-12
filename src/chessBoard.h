#ifndef __CHESSBOARD_H__
#define __CHESSBOARD_H__
#include <memory>
#include <vector>
#include "position.h"
#include "piece.h"
#include "chessSubject.h"

class ChessBoard : public ChessSubject {
  Piece *board[8][8];
  std::vector<std::unique_ptr<Piece>> white;
  std::vector<std::unique_ptr<Piece>> black;
  std::vector<Piece *> removed;
  // std::unique_ptr<ChessLogic> logic;

 public:
  ChessBoard();
  ~ChessBoard();

  void addPiece(const Pos &pos, Piece *piece);
  void removePiece(const Pos &pos);
  Piece *getPiece(const Pos &pos);

  void render();
};

#endif