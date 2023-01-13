#ifndef __CHESSSUBJECT_H__
#define __CHESSSUBJECT_H__
#include <vector>

class Observer;

class ChessSubject {
  std::vector<Observer *> observers;

 public: 
  virtual ~ChessSubject() = default;
  void attach( Observer *o );
  void detach( Observer *o );

 protected:
  void updateObservers();
};

#endif
