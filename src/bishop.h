#ifndef __BISHOP_H__
#define __BISHOP_H__
#include "piece.h"

class Bishop : public Piece {
 public:
	Bishop(char owner);
	bool isValidMove(const Pos &dest) override;
};

#endif