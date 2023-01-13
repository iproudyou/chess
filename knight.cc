#include "chessBoard.h"
#include "knight.h"

Knight::Knight(char owner): Piece{3, 'n', owner} {}

Knight::Knight(char owner, int row, int col): Piece{3, 'n', owner, row, col} {}

bool Knight::isValidMove(ChessBoard* board, const Pos& dest) {
    // Checks if move is not to same position and isWithin the board(can be placed in ChessBoard)
    if (current == dest || !board->isWithin(dest)) {
        return false;
    }

    int rowDiff = abs(current.col - dest.col);
    int colDiff = abs(current.row - dest.row);
    // Checks if dest is a 'L' shape away  
    if ((rowDiff == 1 && colDiff == 2) || (rowDiff == 2 && colDiff == 1)) {
        // Returns true if no friendly piece on dest
        return !board->isFriendly(owner, dest);
    }
    return false;
}
