#ifndef __LEVEL1_H__
#define __LEVEL1_H__

#include "level.h"

class Level1 : public Level{
    public:
        Level1(ChessBoard* b);
        virtual Move getAIMove() override;
};

#endif