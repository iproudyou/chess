#include <iostream>
#include <sstream>
#include "controller.h"
#include "computer.h"
#include "human.h"
#include "textObserver.h"
#include "graphicsObserver.h"
#include "chessBoard.h"

using namespace std;

Controller::Controller( ChessBoard* b ): b{b} {}

void Controller::setUpPlayer(const string& player, int isWhite) {
  if (player == "human") players[isWhite] = make_unique<Human>(!isWhite);
  else if (player == "computer1") players[isWhite] = make_unique<Computer>(!isWhite, 1, b);
  else if (player == "computer2") players[isWhite] = make_unique<Computer>(!isWhite, 2, b);
  else if (player == "computer3") players[isWhite] = make_unique<Computer>(!isWhite, 3, b);
  else if (player == "computer4") players[isWhite] = make_unique<Computer>(!isWhite, 4, b);
  else throw runtime_error("Not a valid player type!");
}

void Controller::playGame() {
  string inputs;
  // Reads initial command inputs
  while(getline(cin, inputs)) {
    try {
      istringstream ss {inputs};
      string command;
      ss >> command;
      // Sets up a new game
      if (command == "game") {
        string player1, player2;
        ss >> player1;
        ss >> player2;
        setUpPlayer(player1, 0);
        setUpPlayer(player2, 1);
        if (isSetup) {
          isSetup = !isSetup; 
        } else {
          o.push_back(make_unique<TextObserver>(b));
          o.push_back(make_unique<GraphicsObserver>(b));
          b->initializeBoard();
        }
        
        start();
        // Checks if user wants to play again, if not exit game
        if (!playAgain()) {
          break;
        }
        o.pop_back();
        o.pop_back();
        b->clearAllBoardState();
      }
      // Configures initial board setup (valid for immediate game)
      else if (command == "setup") {
        o.push_back(make_unique<TextObserver>(b));
        o.push_back(make_unique<GraphicsObserver>(b));
        b->render();
        string setupInputs;
        while (getline(cin, setupInputs)) {
          try {
            istringstream ss {setupInputs};
            ss >> setupInputs;
            // Adds a piece to the board then displays
            if (setupInputs == "+") {
              char piece;
              string pos;
              ss >> piece;
              ss >> pos;
              Pos insertPos {pos};
              b->addPiece(insertPos, piece);
              b->render();
            }
            // Removes a piece to the board then displays 
            else if (setupInputs == "-") {
              string pos;
              ss >> pos;
              Pos removePos {pos};
              b->removeSetupPiece(removePos);
              b->render();
            } 
            // Assigns turn
            else if (setupInputs == "=") {
              string player;
              ss >> player;
              if (player == "white") b->setTurn(0);
              else if (player == "black") b->setTurn(1);
              else throw runtime_error("Not a valid colour!");
            }
            // Checks if user can leave setup 
            else if (setupInputs == "done") {
              if (b->canSetup()) break;
              throw runtime_error("Invalid setup!");
            } 
            // Handles invalid inputs
            else throw runtime_error("Invalid input");
          } catch(runtime_error& e) {
            cerr << e.what() << endl;
          }
        }
        isSetup = true;
      }
      else {
        throw runtime_error("Unrecognized Command!");
      }
    }
    catch (runtime_error& e) {
      cerr << e.what() << endl;
    }
  } 
}

// Starts the game
void Controller::start() {
  string inputs;
  b->render();
  // Main game loop
  while(getline(cin, inputs)) {
    try {
      istringstream ss {inputs};
      ss >> inputs;
      int code = 0;
      if (b->isWhiteTurn()) {
        // White turn
        code = players[0]->move(b, inputs, ss);
      } else {
        // Black's turn 
        code = players[1]->move(b, inputs, ss);
      }

      // check if the game has ended (i.e. checkmate/stalemate)
      if (code == 3 || code == 4) {
        b->printScore();
        break;
      }
      b->render();
    } catch (runtime_error& e) {
      cerr << e.what() << endl;
    }
  }
  
  if (cin.eof()) {
    b->printScore();
    exit(0);
  }
}

bool Controller::playAgain() {
  string inputs;
  char agree;
  cout << "Play another round of chess? (y/n)" << endl << endl;
  while (getline(cin, inputs)) {
    istringstream ss{inputs};
    ss >> agree;
    switch (agree) {
      case 'y':
        return true;
      case 'n':
        return false;
      default:
        cerr << "Not a valid option. Please try again." << endl << endl;
        break;
    }
    cout << "Play another round of chess? (y/n)" << endl << endl;
  }
  return false;
}