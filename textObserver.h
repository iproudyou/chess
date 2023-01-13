#ifndef __TEXTOBSERVER_H__
#define __TEXTOBSERVER_H__
#include <iostream>
#include "observer.h"

class ChessBoard;

class TextObserver : public Observer {
  ChessBoard *b;
  std::ostream &out = std::cout;

 public:
  TextObserver(ChessBoard *b);
  ~TextObserver();
  virtual void update() override;
};

#endif