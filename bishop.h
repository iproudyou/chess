#ifndef __BISHOP_H__
#define __BISHOP_H__

#include "piece.h"

class Bishop : public Piece {
    public:
        Bishop(char owner);
        Bishop(char owner, int row, int col);
        virtual bool isValidMove(ChessBoard* board, const Pos& dest) override;
};

#endif
