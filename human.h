#ifndef __HUMAN_H__
#define __HUMAN_H__

#include "player.h"

class ChessBoard;

class Human : public Player {
    public:
        explicit Human(bool isWhite);
        // 1 = legal move, 2 = illegal move, 3 = checkmate, 4 = stalemate
        virtual int move(ChessBoard* b, const std::string& inputs, std::istringstream& ss) override;
};

#endif