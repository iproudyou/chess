#include <memory>
#include "chessBoard.h"
#include "king.h"
#include "queen.h"
#include "bishop.h"
#include "rook.h"
#include "knight.h"
#include "pawn.h"

using namespace std;

// create a set of black and white pieces
ChessBoard::ChessBoard() {
}

ChessBoard::~ChessBoard() {}

void ChessBoard::addPiece(const Pos &pos, Piece *piece) {
  board[pos.row][pos.col] = piece;
}

void ChessBoard::removePiece(const Pos &pos) {
  board[pos.row][pos.col] = nullptr;
}

Piece * ChessBoard::getPiece(const Pos &pos) {
  return board[pos.row][pos.col];
}

void ChessBoard::render() {
  updateObservers();
}
