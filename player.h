#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <string>
#include <sstream>

class ChessBoard;
class Pos;

class Player {
    protected:
        bool isWhite;
    public: 
        Player(bool isWhite);
        virtual ~Player();
        // 1 = legal move, 2 = illegal move, 3 = checkmate, 4 = stalemate
        virtual int move(ChessBoard* board, const std::string& inputs, std::istringstream& ss) = 0;
};

#endif