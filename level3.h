#ifndef __LEVEL3_H__
#define __LEVEL3_H__

#include "level.h"

class Level3 : public Level{
    public:
        Level3(ChessBoard* b);
        virtual Move getAIMove() override;
};

#endif