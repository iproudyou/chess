#include "controller.h"
#include "chessBoard.h"

int main() {
  ChessBoard b; // creates a chessboard (i.e., Model)
  Controller c{&b}; // creates controller
  c.playGame();
}