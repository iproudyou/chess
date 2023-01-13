#ifndef __PAWN_H__
#define __PAWN_H__

#include "piece.h"

class Pawn : public Piece {
    public:
        Pawn(char owner);
        Pawn(char owner, int row, int col);
        virtual bool isValidMove(ChessBoard* board, const Pos& dest) override;
};

#endif