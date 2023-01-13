#ifndef __KNIGHT_H__
#define __KNIGHT_H__

#include "piece.h"

class Knight : public Piece {
    public:
        Knight(char owner);
        Knight(char owner, int row, int col);
        virtual bool isValidMove(ChessBoard* board, const Pos& dest) override;
};

#endif