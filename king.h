#ifndef __KING_H__
#define __KING_H__

#include "piece.h"

class King : public Piece {
    public:
        King(char owner);
        King(char owner, int row, int col);
        virtual bool isValidMove(ChessBoard* board, const Pos& dest) override;
};

#endif