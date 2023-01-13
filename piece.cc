#include "piece.h"
#include "position.h"

Piece::Piece(int val, char piece, char owner): 
val{val}, piece{piece}, current{Pos{}}, owner{owner} {}

Piece::Piece(int val, char piece, char owner, int row, int col): 
val{val}, piece{piece}, current{Pos{row, col}}, owner{owner} {}

Piece::~Piece() {}

int Piece::getVal() const { return val; }

char Piece::getOwner() const { return owner; }

char Piece::getPiece() const { return piece; }

const Pos & Piece::getPos() const { return current; }

void Piece::setPos(const Pos pos) {
    current.row = pos.row;
    current.col = pos.col;
}

bool Piece::didMove() {
    return totalMoves > 0;
}

void Piece::incrementMoves() {
    ++totalMoves;
}

void Piece::decrementMoves() {
    --totalMoves;
}