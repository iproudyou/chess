#include "chessBoard.h"
#include "rook.h"
#include <iostream>

Rook::Rook(char owner): Piece{5, 'r', owner} {}

Rook::Rook(char owner, int row, int col): Piece{5, 'r', owner, row, col} {}

bool Rook::isValidMove(ChessBoard* board, const Pos& dest) {
    if (current == dest || !board->isWithin(dest)) {
        return false;
    }
    int start, end;
    
    // Checks vertical movement 
    if (dest.col == current.col) {
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