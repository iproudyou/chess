#include "level3.h"
#include "chessBoard.h"
#include "chessMove.h"

using namespace std;

Level3::Level3(ChessBoard* b) : Level(b) {}
Move Level3::getAIMove() {
    bool isWhite = b->isWhiteTurn();
    vector<Move> possibleMoves = b->getAllValidMoves(isWhite);
    Move aiMove;

    for (auto & move : possibleMoves) {
        if (b->getThreats(move.from, isWhite).size() > 0 && b->getThreats(move.to, isWhite).size() == 0) {
            aiMove = move;
            return aiMove;
        }
    }

    if (!preferCaptureCheck(aiMove, possibleMoves, isWhite)) {
        aiMove = generateRandomMove(possibleMoves);
    }
    return aiMove;
}