#include "piece.h"

Piece::Piece(int val, char piece, char owner): 
val{val}, piece{piece}, owner{owner} {}

Piece::~Piece() {}

int Piece::getVal() const {
  return val;
}

char Piece::getOwner() const {
  return owner;
}

char Piece::getPiece() const {
  return piece;
}