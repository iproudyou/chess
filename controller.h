#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <string>
#include <memory>
#include <vector>
#include "observer.h"
#include "player.h"

class ChessBoard;

class Controller {
  bool isSetup;
  ChessBoard *b;
  std::vector<std::unique_ptr<Observer>> o;
  std::unique_ptr<Player> players [2];

 public:
  Controller( ChessBoard* b);
  void playGame();
  void start();
  void setUpPlayer(const std::string& player, int isWhite);
  bool playAgain();
};

#endif
