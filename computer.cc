#include "computer.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
#include "level4.h"
#include "chessMove.h"
#include "chessBoard.h"

using namespace std;

Computer::Computer(bool isWhite, int level, ChessBoard* b) : Player(isWhite) {
    switch(level) {
        case 1:
            // Computer player plays random move
            ai = make_unique<Level1>(b);
            break;
        case 2:
            // Computer player prefers captures and checks
            ai = make_unique<Level2>(b);
            break;
        case 3:
            // Computer player prefers avoiding capture, captures, and checks
            ai = make_unique<Level3>(b);
            break;
        case 4:
            // Computer player plays sophisticated moves
            ai = make_unique<Level4>(b);
            break;
    }
}

int Computer::move(ChessBoard* b, const string& inputs, istringstream& ss) { 
    // Computer player makes a move
    if (inputs == "move") {
        Move aiMove = ai.get()->getAIMove(); 
        if (aiMove.action == "promotion") {
            // ' ' automatically changes to queen
            return b->move(aiMove.from, aiMove.to, ' ');
        }
        else {
            return b->move(aiMove.from, aiMove.to);
        }
    } 
    // Computer player resigns
    else if (inputs == "resign") {
        b->resign();
        return 3;
    } 
    // Computer player undo a move
    else if (inputs == "undo") {
        b->undoMove();
    }
    else {
        throw runtime_error("Illegal Move"); 
    }
    return 0;
}