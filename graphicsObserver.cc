#include "graphicsObserver.h"
#include "chessBoard.h"
#include "chessWindow.h"

using namespace std;

GraphicsObserver::GraphicsObserver(ChessBoard *b): b{b}, window{make_unique<ChessWindow>()} {
	b->attach( this );
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			isPiece[i][j] = ' ';	
		}
	}
}

GraphicsObserver::~GraphicsObserver() {
	b->detach( this );
}

// Modify to call function from XWindow
void GraphicsObserver::update() {
	Pos p {};
	for (int row = 0; row < 8; ++ row) {
		for (int col = 0; col < 8; ++col) {
			p.row = row;
			p.col = col;
			Piece *piece = b->getPiece(p);
			if (piece != nullptr) {
				if (piece->getOwner() == 'w' && isPiece[row][col] != toupper(piece->getPiece())) {
					char upperPiece = toupper(piece->getPiece());
					isPiece[row][col] = upperPiece;
					window->draw(upperPiece, p);
				} else if(piece->getOwner() == 'b' && isPiece[row][col] != piece->getPiece()) {
					isPiece[row][col] = piece->getPiece();
					window->draw(piece->getPiece(), p);
				}
			} else {
				isPiece[row][col] = ' ';
				window->clear(p);
			}
			
		}
	}
}