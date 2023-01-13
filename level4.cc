#include "level4.h"
#include "chessBoard.h"
#include "chessMove.h"
#include <iostream>

#define INT_MAX 2000000000
#define INT_MIN -2000000000

using namespace std;

Level4::Level4(ChessBoard* b) : Level(b) {}

pair<Move, int> Level4::minimax(int depth, bool maxPlayer, char maxColour, bool turn) {
    
    if (depth == 0) {
        return make_pair(Move {}, evaluateBoard(maxColour));
    }

    vector<Move> possibleMoves = b->getAllValidMoves(turn);
    Move aiMove = generateRandomMove(possibleMoves);
    int currentEval;
    int minMaxEval = maxPlayer ? INT_MIN : INT_MAX;
    
    for (auto & move : possibleMoves) {
        b->simulateMove(move);
        currentEval = minimax(depth - 1, !maxPlayer, maxColour, !turn).second;
        b->revertSimulation(move);
        if (maxPlayer) {
            if (currentEval > minMaxEval) {
                minMaxEval = currentEval;
                aiMove = move;
            }
        } else {
            if (currentEval < minMaxEval) {
                minMaxEval = currentEval;
                aiMove = move;
            }
        }
        
    }
    return make_pair(aiMove, minMaxEval);
}

Move Level4::getAIMove() { 
    return minimax(depth, true, b->isWhiteTurn() ? 'w' : 'b', b->isWhiteTurn()).first;
}