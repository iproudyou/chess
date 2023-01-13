#ifndef __COMPUTER_H__
#define __COMPUTER_H__

#include <memory>
#include "player.h"
#include "level.h"

class ChessBoard;

class Computer : public Player {
    std::unique_ptr<Level> ai;
    public:
        Computer(bool isWhite, int level, ChessBoard* b);
        // 1 = legal move, 2 = illegal move, 3 = checkmate, 4 = stalemate
        virtual int move(ChessBoard* b, const std::string& inputs, std::istringstream& ss) override;
};

#endif