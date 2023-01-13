#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <vector>
#include <map>

class Move;
class ChessBoard;
class Piece;

class Level {

    protected:
        ChessBoard *b;
        // As we are not chess experts, we have adapted the piece square table
        // from https://adamberent.com/2019/03/02/piece-square-table/, these tables 
        // only serve to enhance the performance of the engine. It does not affect
        // nor significantly influence our code in any way.
        std::map<char,int> weightMap;
        const short pawnTable[64];
        const short knightTable[64]; 
        const short bishopTable[64];
        const short kingTable[64];

        Move generateRandomMove(const std::vector<Move>& possibleMoves);
        bool leadsToCheck(const Move& move, bool isWhiteTurn);
        bool preferCaptureCheck(Move& aiMove, const std::vector<Move>& possibleMoves, bool isWhite);
        int evaluateBoard(char maxColour);
        int evaluatePiece(Piece* piece);
    public:
        Level(ChessBoard* b);
        virtual ~Level();
        virtual Move getAIMove() = 0;
};

#endif