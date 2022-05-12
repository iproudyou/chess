#ifndef __PAWN_H__
#define __PAWN_H__
#include "piece.h"

class Pawn : public Piece {
 public:
	Pawn(char owner);
	bool isValidMove(const Pos &dest) override;
};

#endif