#include "level1.h"
#include "chessBoard.h"
#include "chessMove.h"

using namespace std;

Level1::Level1(ChessBoard* b) : Level(b) {}
Move Level1::getAIMove() {
    vector<Move> possibleMoves = b->getAllValidMoves(b->isWhiteTurn());
    return generateRandomMove(possibleMoves);
}