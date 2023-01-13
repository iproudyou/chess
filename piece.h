#ifndef __PIECE_H__
#define __PIECE_H__

// #include <memory>
// #include <vector>
#include "position.h"

class ChessBoard;

class Piece {
  int val;
  char piece;

  protected:
    Pos current;
    char owner;
    int totalMoves = 0;

  public:
    Piece(int val, char piece, char owner);
    Piece(int val, char piece, char owner, int row, int col);
    virtual ~Piece();
    virtual bool isValidMove(ChessBoard* board, const Pos& dest) = 0;
    int getVal() const;
    char getOwner() const;
    char getPiece() const;
    const Pos & getPos() const;
    void setPos(const Pos pos);

    bool didMove();
    void incrementMoves();
    void decrementMoves();
};

#endif
