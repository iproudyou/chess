#ifndef __PIECE_H__
#define __PIECE_H__

// #include <memory>
// #include <vector>
#include "chessBoard.h"
#include "position.h"

// using std::vector;
// using std::shared_ptr;

class Piece {
	// int val;
	char piece;
	char owner;
	Pos current;
	// vector<shared_ptr<Piece>> threats;

 public:
	Piece(int val, char piece, char owner);
	virtual ~Piece();

	virtual bool isValidMove(const Pos &dest) = 0;
	// void setThreats();
	// vector<shared_ptr<Piece>>& getThreats();

	int getVal();
	char getOwner();
	char getPiece();
};

#endif