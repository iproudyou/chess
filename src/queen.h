#ifndef __QUEEN_H__
#define __QUEEN_H__
#include "piece.h"

class Queen : public Piece {
 public:
	Queen(char owner);
	bool isValidMove(const Pos &dest) override;
};

#endif