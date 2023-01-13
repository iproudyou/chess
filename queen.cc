#include "chessBoard.h"
#include "queen.h"

Queen::Queen(char owner): Piece{9, 'q', owner} {}

Queen::Queen(char owner, int row, int col): Piece{9, 'q', owner, row, col} {}

bool Queen::isValidMove(ChessBoard* board, const Pos& dest) {
    // checks if move is not to same position and isWithin the board(can be placed in ChessBoard)
    if (current == dest || !board->isWithin(dest)) {
        return false;
    }
    int start, end;
    int rowDiff = dest.row - current.row;
    int colDiff = dest.col - current.col;

    // Checks diagonal movement
    if (abs(rowDiff) == abs(colDiff)) {
        int rowDir = rowDiff > 0 ? 1 : -1;
        int colDir = colDiff > 0 ? 1 : -1;

        Pos temp {current.row, current.col};
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
    // Checks vertical movement 
    else if (dest.col == current.col) {
        start = dest.row < current.row ? dest.row : current.row;
        end = dest.row < current.row ? current.row : dest.row;

        Pos temp {start, dest.col};
        for (int i = start + 1; i < end; ++i) {
            temp.row = i;
            if (i != current.row && board->isObstacle(temp)) {
                return false;
            }
        }
        // Reachable if dest does not have a friendly piece
        return !board->isFriendly(owner, dest);
    }
    // Checks horizontal movement  
    else if (dest.row == current.row) {
        start = dest.col < current.col ? dest.col : current.col;
        end = dest.col < current.col ? current.col : dest.col;
    
        Pos temp {dest.row, start};
        for (int i = start + 1; i < end; ++i) {
            temp.col = i;
            if (i != current.col && board->isObstacle(temp)) {
                return false;
            }
        }
        // Reachable if dest does not have a friendly piece
        return !board->isFriendly(owner, dest);
    }
    return false;
}
