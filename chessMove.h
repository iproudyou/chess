#ifndef __MOVE_H__
#define __MOVE_H__
#include "position.h"
#include <string>

class Piece;

struct Move {
    Pos from;
    Pos to;
    Piece *captured;
    Piece *moved;
    std::string action; // castling, en-passant, capture, move, promotion, illegal
};

#endif
