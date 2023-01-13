#ifndef __LEVEL4_H__
#define __LEVEL4_H__

#include "level.h"
#include <utility>

class Level4 : public Level {

    int depth = 3;
    
    public:
        std::pair<Move, int> minimax(int depth, bool maxPlayer, char maxColour, bool turn);
        Level4(ChessBoard* b);
        virtual Move getAIMove() override;
};

#endif