#ifndef __GRAPHICSOBSERVER_H__
#define __GRAPHICSOBSERVER_H__
#include <iostream>
#include <memory>
#include "observer.h"

class ChessWindow;
class ChessBoard;

class GraphicsObserver : public Observer {
  ChessBoard *b;
  std::unique_ptr<ChessWindow> window; // window to display canvas on
  std::ostream &out = std::cout;
  char isPiece[8][8];

 public:
  GraphicsObserver(ChessBoard *b);
  ~GraphicsObserver();
  virtual void update() override;
};

#endif