#include "chessSubject.h"
#include "observer.h"
#include <vector>
#include <iostream>
void ChessSubject::attach( Observer *o ) {
  observers.emplace_back(o);
}

void ChessSubject::detach( Observer *o ) {
  for (auto it = observers.begin(); it != observers.end(); ++it) {
    if (*it == o) {
      observers.erase(it);
      break;
    }
  }
}

void ChessSubject::updateObservers() {
  for (auto &ob : observers) ob->update();
}
