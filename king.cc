#include "chessBoard.h"
#include "king.h"

King::King(char owner): Piece{0, 'k', owner} {}

King::King(char owner, int row, int col): Piece{0, 'k', owner, row, col} {}

bool King::isValidMove(ChessBoard* board, const Pos& dest) {
    if (current == dest || !board->isWithin(dest)) {
        return false;
    }
    
    int rowDiff = dest.row - current.row;
    int colDiff = dest.col - current.col;

    if (abs(rowDiff) <= 1 && abs(colDiff) <= 1 && !board->isFriendly(getOwner(), dest)) 
        return true;
        
    return false;
}
