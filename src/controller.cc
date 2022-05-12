#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <utility>
#include <vector>
#include "controller.h"
#include "observer.h"
#include "textObserver.h"
#include "chessBoard.h"
using namespace std;

void Controller::playGame() {
  // GameController g;
  ChessBoard b;
  string inputs;

  vector<unique_ptr<Observer>> o;

  try {    
    while( getline(cin, inputs) ) {  
      istringstream ss {inputs};
      string command;
      ss >> command;

      if (command == "render") {
        // this command is to be deleted; use it for a testing purpose only
        b.render();
      }       
      else if (command == "move") {
        // to start with, consider we are having human vs human
        string from, to;
        ss >> from;
        ss >> to;
        // need to consider an extra read-value for pawn promotion
        // move for computer does not have from to read-values
      }
      else if (command == "game") {
        string player1, player2;
        ss >> player1;
        ss >> player2;
        auto text = make_unique<TextObserver>(&b);
        o.push_back(move(text));
        // g.setUpPlayers(player1, player2);
        // g.playGame();
      }
      else if (command == "resign") {
        // g.resign();
        break;
      }
      else if (command == "setup") {
        // g.setup();
      }
      else {
        throw runtime_error("Unrecognized Command");
      }
    }
  } catch (runtime_error& e) {
    cerr << e.what() << endl;
  }
}
