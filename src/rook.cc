#include "rook.h"

Rook::Rook(char owner): Piece{5, 'r', owner} {}

bool Rook::isValidMove(ChessBoard* board, const Pos &dest) {
    if (current == dest || !board->isWithin(dest)) {
        return false;
    }
    int start, end;
    if (dest.row == current.row) {
        start = dest.col < current.col ? dest.col : current.col;
        end = dest.col < current.col ? current.col : dest.col;
    
        Pos temp {dest.row, start};
        for (int i = start; i <= end; ++i) {
            temp.col++;

            if (board->isFriendly(owner, temp)) {
                return false;
            }
        }
        return true;
    } else if (dest.col == current.col) {
        start = dest.row < current.row ? dest.row : current.row;
        end = dest.row < current.row ? current.row : dest.row;
    
        Pos temp {start, dest.col};
        for (int i = start; i <= end; ++i) {
            temp.row++;
            if (board->isFriendly(owner, temp)) {
                return false;
            }
        }
        return true;
    }
    return false;
}