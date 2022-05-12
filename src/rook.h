#ifndef __ROOK_H__
#define __ROOK_H__
#include "piece.h"

class Rook : public Piece {
 public:
	Rook(char owner);
	bool isValidMove(const Pos &dest) override;
};

#endif