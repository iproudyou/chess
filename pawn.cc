#include "chessBoard.h"
#include "pawn.h"
#include <iostream>

Pawn::Pawn(char owner): Piece{1, 'p', owner} {}

Pawn::Pawn(char owner, int row, int col): Piece{1, 'p', owner, row, col} {}

bool Pawn::isValidMove(ChessBoard* board, const Pos& dest) {
    if (current == dest || !board->isWithin(dest)) {
        return false;
    }

    // Pawn's action on different columns
    if (current.col != dest.col) {
        
        // More than one column apart
        if (abs(current.col - dest.col) > 1 ) {
            return false;
        }
        // More than 1 row apart 
        else if (abs(current.row - dest.row) > 1) {
            return false;
        }
        // Checks if diagonal from pawn is an enemy piece 
        else {
            // Checks if there is an enemy piece one row above (white)
            if (owner == 'w' && dest.row == current.row - 1 && board->isEnemy(owner, dest)) {
                return true;
            }
            // Checks if there is an enemy piece one row below (black)
            else if (owner == 'b' && dest.row == current.row + 1 && board->isEnemy(owner, dest)) {
                return true;
            }
            return false;
        }
    } 
    
    // Forward movement (white)
    if (owner == 'w') {
        // Moves two spots if no obstructions
        if (!didMove() && dest.row == current.row - 2) {
            Pos temp1 {dest.row, dest.col};
            Pos temp2 {current.row - 1, dest.col};
            return !(board->isObstacle(temp1) || board->isObstacle(temp2));
        }
        // Moves one spot if no obstructions 
        else if (dest.row == current.row - 1) {
            Pos temp1 {dest.row, dest.col};
            return !board->isObstacle(temp1);
        }
    }
    // Forward movement (black) 
    else {
        // Moves two spots if no obstructions
        if (!didMove() && dest.row == current.row + 2) {
            Pos temp1 {dest.row, dest.col};
            Pos temp2 {current.row + 1, dest.col};
            return !(board->isObstacle(temp1) || board->isObstacle(temp2));
        }
        // Moves one spot if no obstructions  
        else if (dest.row == current.row + 1) {
            Pos temp1 {dest.row, dest.col};
            return !board->isObstacle(temp1);
        }
    }

    return false;
}
