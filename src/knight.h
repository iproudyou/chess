#ifndef __KNIGHT_H__
#define __KNIGHT_H__
#include "piece.h"

class Knight : public Piece {
 public:
	Knight(char owner);
	bool isValidMove(const Pos &dest) override;
};

#endif