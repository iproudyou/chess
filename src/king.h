#ifndef __KING_H__
#define __KING_H__
#include "piece.h"

class King : public Piece {
 public:
	King(char owner);
	bool isValidMove(const Pos &dest) override;
};

#endif