#ifndef __ROOK_H__
#define __ROOK_H__

#include "piece.h"

class Rook : public Piece {
    public:
        Rook(char owner);
        Rook(char owner, int row, int col);
        virtual bool isValidMove(ChessBoard* board, const Pos& dest) override;
};

#endif