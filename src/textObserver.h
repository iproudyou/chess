#ifndef __TEXTOBSERVER_H__
#define __TEXTOBSERVER_H__
#include <iostream>
#include "observer.h"

class ChessBoard;

class TextObserver : public Observer {
  ChessBoard *board;
  std::ostream &out = std::cout;

 public:
  TextObserver(ChessBoard *board);
  ~TextObserver();
  void update() override;
};

#endif