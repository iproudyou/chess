#ifndef __QUEEN_H__
#define __QUEEN_H__

#include "piece.h"

class Queen : public Piece {
    public:
        Queen(char owner);
        Queen(char owner, int row, int col);
        virtual bool isValidMove(ChessBoard* board, const Pos& dest) override;
};

#endif