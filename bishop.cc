#include "chessBoard.h"
#include "bishop.h"

Bishop::Bishop(char owner) : Piece{3, 'b', owner} {}

Bishop::Bishop(char owner, int row, int col) : Piece{3, 'b', owner, row, col} {}

bool Bishop::isValidMove(ChessBoard* board, const Pos& dest) {
    // checks if move is not to same position and isWithin the board(can be placed in ChessBoard)
    if (current == dest || !board->isWithin(dest)) {
        return false;
    }

    int rowDiff = dest.row - current.row;
    int colDiff = dest.col - current.col;

    // Not on a diagonal (not reachable)
    if (abs(rowDiff) != abs(colDiff)) {
        return false;
    }

    int rowDir = rowDiff > 0 ? 1 : -1;
    int colDir = colDiff > 0 ? 1 : -1;

    Pos temp {current.row, current.col};

    // Checks diagonal for obstructions
    for (int i = 0; i < abs(rowDiff) - 1; ++i) {
        temp.row += rowDir;
        temp.col += colDir;

        if (board->isObstacle(temp)) {
            return false;
        }
    }
    // Reachable if dest does not have a friendly piece
    return !board->isFriendly(owner, dest);
}
