#include "textObserver.h"
#include "chessBoard.h"
#include "observer.h"
TextObserver::TextObserver(ChessBoard *board): board{board} {
  board->attach( this );
}

TextObserver::~TextObserver() {
  board->detach( this );
}

void TextObserver::update() {
  out << "OBSERVER !" << std::endl;
  
  // out << '+';
  // for (int j = l; j < r; ++j) out << '-';
  // out << '+' << std::endl;
  // for (int i = t; i < b; ++i) {
  //   out << '|';
  //   for (int j = l; j < r; ++j) {
  //     out << subject->getState(i, j);
  //   }
  //   out << '|' << std::endl;
  // }
  // out << '+';
  // for (int j = l; j < r; ++j) out << '-';
  // out << '+' << std::endl;
}
