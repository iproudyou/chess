#ifndef __LEVEL2_H__
#define __LEVEL2_H__

#include "level.h"

class Level2 : public Level {
    public:
        Level2(ChessBoard* b);
        virtual Move getAIMove() override;
};

#endif