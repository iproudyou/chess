#include "human.h"
#include "position.h"
#include "chessBoard.h"

using namespace std;

Human::Human(bool isWhite) : Player(isWhite) {}
int Human::move(ChessBoard *b, const string& inputs, istringstream& ss) {
    // Human player makes a move
    if (inputs == "move") {
        string from, to;
        ss >> from;
        ss >> to;
        Pos startPos {from};
        Pos endPos {to};

        char prom;
        if (ss >> prom) {
            return b->move(startPos, endPos, prom);
        }
        else {
            return b->move(startPos, endPos);
        }
    } 
    // Human player resigns
    else if (inputs == "resign") {
        b->resign();
        return 3;
    } 
    // Human player undo a move
    else if (inputs == "undo") {
        b->undoMove();
    }
    else {
        throw runtime_error("Illegal Move"); 
    }
    return 0;
}