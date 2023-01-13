#include "level2.h"
#include "chessBoard.h"
#include "chessMove.h"

using namespace std;

Level2::Level2(ChessBoard* b) : Level(b) {}
Move Level2::getAIMove() {
    bool isWhite = b->isWhiteTurn();
    vector<Move> possibleMoves = b->getAllValidMoves(isWhite);
    Move aiMove;
    
    if (!preferCaptureCheck(aiMove, possibleMoves, isWhite)) {
        aiMove = generateRandomMove(possibleMoves);
    }

    return aiMove;
}