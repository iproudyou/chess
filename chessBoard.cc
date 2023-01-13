#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <algorithm>
#include <utility>
#include "chessMove.h"
#include "chessBoard.h"
#include "piece.h"
#include "king.h"
#include "queen.h"
#include "bishop.h"
#include "rook.h"
#include "knight.h"
#include "pawn.h"
#include "position.h"

using namespace std;

#define FEN_STRING "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

// create a set of black and white pieces
ChessBoard::ChessBoard() : turn{0}, score{0,0} {
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      board[i][j] = nullptr;
    }
  }
}

ChessBoard::~ChessBoard() {}

void ChessBoard::initializeBoard() {
  istringstream ss{FEN_STRING};
  char c;
  char owner;
  int row = 0;
  int col = 0;
  unique_ptr<Piece> p{nullptr};

  while (ss >> c) {
    // Goes to new row
    if (c == '/') {
      col = 0;
      ++row;
    }
    else if ((c >= '0') && (c <= '9')) {
      for (int i = 0; i < c - '0'; ++i) {
        ++col;
      }
    }
    else { 
      createPiece(owner, c, row, col, p);
      ++col;
    }
  }
}

void ChessBoard::createPiece(char& owner, char piece, int row, int col, unique_ptr<Piece>& p) {
  // Black piece
  if (piece == tolower(piece)) {
    owner = 'b';
  }
  // White piece
  else if (piece == toupper(piece)) {
    owner = 'w';
    piece = tolower(piece);
  } 
  // Handles invalid piece
  else throw("Invalid Piece!");

  // Selects which piece to generate
  switch (piece) {
    case 'r':
      p = make_unique<Rook>(owner, row, col);
      break;
    case 'n':
      p = make_unique<Knight>(owner, row, col);
      break; 
    case 'b':
      p = make_unique<Bishop>(owner, row, col);
      break; 
    case 'q':
      p = make_unique<Queen>(owner, row, col);
      break; 
    case 'k':
      p = make_unique<King>(owner, row, col);
      break; 
    case 'p':
      p = make_unique<Pawn>(owner, row, col);
      break; 
    default:
      break;
  }
  // insert the king to the front, but rest to the end
  if (piece == 'k') {
    owner == 'b' ? black.insert(black.begin(), std::move(p)) : white.insert(white.begin(), std::move(p)); 
    board[row][col] = owner == 'b' ? black.front().get() : white.front().get();
  }
  else {
    owner == 'b' ? black.push_back(std::move(p)) : white.push_back(std::move(p)); 
    board[row][col] = owner == 'b' ? black.back().get() : white.back().get();
  }
}

// ---------------------------- for setup -----------------------------

void ChessBoard::addPiece(const Pos& pos, char piece) {
  char owner;
  if (getPiece(pos) != nullptr) removePiece(pos, true);
  unique_ptr<Piece> p = { nullptr };
  createPiece(owner, piece, pos.row, pos.col, p);
}

void ChessBoard::removeSetupPiece(const Pos pos) {
  Piece *p = getPiece(pos);
  if (p == nullptr) return;
  Pos tmp {-1, -1};
  p->setPos(tmp);
  board[pos.row][pos.col] = nullptr;
}

// --------------------------------------------------------------------

void ChessBoard::clearAllBoardState() {
  // clear the board
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      board[i][j] = nullptr;
    }
  }

  // destroys elements in white, black, and record
  white.clear();
  black.clear();
  record.clear();

  // set the turn to 0
  turn = 0;
}

void ChessBoard::addPiece(const Pos &pos, Piece *piece) {
  board[pos.row][pos.col] = piece;
  piece->setPos(pos);
  piece->incrementMoves();
}

void ChessBoard::addPieceNoIncrement(const Pos &pos, Piece *piece) {
  board[pos.row][pos.col] = piece;
  piece->setPos(pos);
}

// Clears away pieces that have position {-1, -1}
void ChessBoard::clearEmptyPieces() {
  Pos tmp{-1, -1};

  white.erase(
    std::remove_if( 
      white.begin(),
      white.end(),
      [&] (std::unique_ptr<Piece> const& p)
      { 
        return p.get()->getPos() == tmp;
      }),
    white.end()
  ); 

  black.erase(
    std::remove_if( 
      black.begin(),
      black.end(),
      [&] (std::unique_ptr<Piece> const& p)
      { 
        return p.get()->getPos() == tmp;
      }),
    black.end()
  ); 
}

void ChessBoard::removePiece(const Pos pos, bool capture) {
  if (capture) {
    Piece* p1 = getPiece(pos);
    Pos tmp {-1, -1};
    p1->setPos(tmp);
  }
  board[pos.row][pos.col] = nullptr;
}

Piece * ChessBoard::getPiece(const Pos &pos) {
  return board[pos.row][pos.col];
}

Move ChessBoard::isValidMove(const Pos &from, const Pos &to) {
  // there is a validation and an actual change for each move
  // in isValidMove, we will only output the validation
  Move m;
  Piece *piece = getPiece(from);

  if (piece == nullptr) {
    m.action = "illegal";
    return m;
  }

  m.moved = piece;
  m.from = from;
  m.to = to;

  bool isPawn = piece->getPiece() == 'p';
  bool isKing = piece->getPiece() == 'k';
  bool isBlack = piece->getOwner() == 'b';
  bool isWhite = piece->getOwner() == 'w';
  bool isPieceValidMove = piece->isValidMove(this, to);

  // 1. en-passant
  if (isPawn && canEnPassant(piece, from, to)) {
    Pos enemyPawnPos{from.row, to.col};
    m.captured = getPiece(enemyPawnPos);
    m.action = "en-passant";
  }
  // 2. promote
  else if (isPawn && isPieceValidMove && ((from.row == 1 && to.row == 0 && isWhite) || (from.row == 6 && to.row == 7 && isBlack))) {
    m.captured = isEnemy(piece->getOwner(), to) ? getPiece(to) : nullptr;
    m.action = "promotion";
  }
  // 3. castling (two cases: left and right)
  else if (isKing && !isPieceValidMove) {
    // need to check if the king and rook are in our desired position
    char owner = piece->getOwner();
    int castleRow = owner == 'w' ? 7 : 0;
    Pos pos1 { castleRow , 0};  
    Pos pos2 { castleRow , 7}; 
    Piece * rook1 = getPiece(pos1);
    Piece * rook2 = getPiece(pos2);
    bool isIllegal = true;
    bool isKingInCorrectPos = castleRow == from.row && castleRow == to.row && from.col == 4;
    // Left castle 
    if (isKingInCorrectPos && canCastle(piece, rook1)) {
      if (to.col == 2) {
        m.captured = rook1;
        m.action = "castling";
        isIllegal = false;
      }
    }
    // Right castle
    if (isKingInCorrectPos && canCastle(piece, rook2)) {
      if (to.col == 6) {
        m.captured = rook2;
        m.action = "castling";
        isIllegal = false;
      }
    }
    if (isIllegal) m.action = "illegal";
  }
  // 4. regular move
  else if (isPieceValidMove) {
    if (isEnemy(piece->getOwner(), to)) {
      m.captured = getPiece(to);
      m.action = "capture";
    }
    else {
      m.action = "move";
    }
  }
  // 5. illegal move
  else {
    m.action = "illegal";
  }

  return m;
}

void ChessBoard::simulateMove(const Move &m, char prom) {
  if (m.action == "en-passant") {
    removePiece(m.from);
    removePiece(m.captured->getPos(), true);
    addPieceNoIncrement(m.to, m.moved);
  }
  else if (m.action == "castling") {
    Pos RookBeforePos = m.captured->getPos();
    Pos RookAfterPos{RookBeforePos.row, RookBeforePos.col == 0 ? 3 : 5};

    removePiece(RookBeforePos); // removes original rook pos
    removePiece(m.from); // removes original king pos
    addPieceNoIncrement(RookAfterPos, m.captured); // add rook to rookNew pos
    addPieceNoIncrement(m.to, m.moved); // add king to 'to' pos
  }
  else if (m.action == "move") {
    removePiece(m.from);
    addPieceNoIncrement(m.to, m.moved);
  }
  else if (m.action == "capture") {
    removePiece(m.from);
    removePiece(m.to, true);
    addPieceNoIncrement(m.to, m.moved);
  } else if (m.action == "promotion") {
    // for computer promotion, pawn automatically gets converted to a Queen
    if (prom == ' ') {
      char owner = m.moved->getOwner();
      prom = owner == 'b' ? 'q' : 'Q';
    }
    promote(m.from, m.to, prom);
  }
}

int ChessBoard::checkGameStatus() {
  if (isCheck(isWhiteTurn())) {
    if (isCheckmate(isWhiteTurn())) {
      if (isWhiteTurn()) {
        ++score[1];
        cout << "Checkmate! Black wins!" << endl << endl;
      }
      else {
        ++score[0];
        cout << "Checkmate! White wins!" << endl << endl;
      }
      return 3;
    }
    if (isWhiteTurn()) cout << "White is in check." << endl << endl;
    else cout << "Black is in check." << endl << endl;
  }
  else if (isStalemate(isWhiteTurn())) {
    score[0] = score[0] + 0.5;
    score[1] = score[1] + 0.5;
    cout << "Stalemate!" << endl;
    return 4;
  }

  return 1;
}

// handles en-passant, castling, move, capture
int ChessBoard::move(const Pos &from, const Pos &to) {
  bool validTurn = isValidTurn(from);

  if (!validTurn) {
    cerr << "Wrong player's turn to move" << endl;
    return 2;
  }
  
  Move m = isValidMove(from, to);

  if (m.action == "illegal") {
    cerr << "Illegal move" << endl;
    return 2;
  }

  if (willLeadToCheck(m)) {
    cerr << "This move exposes your king to a check" << endl;
    return 2;
  }

  // simulate the move
  simulateMove(m);

  // increase the totalMove in the piece
  m.moved->incrementMoves();

  // only castling needs its captured piece (i.e. rook) to increase the totalMove as well
  if (m.action == "castling") m.captured->incrementMoves();

  // adds the move to record
  record.push_back(m);

  // takes a turn
  nextTurn();
  
  // remember that we took the turn in nextTurn()
  // for example, assume this move function call was called on a black piece
  // but now, it will be a white's turn
  return checkGameStatus();
}

// only handles promotion
int ChessBoard::move(const Pos &from, const Pos &to, char prom) {
  bool validTurn = isValidTurn(from);
  
  if (!validTurn) {
    cerr << "Wrong player's turn to move" << endl;
    return 2;
  }

  Move m = isValidMove(from, to);
  
  if (m.action == "illegal") {
    cerr << "Illegal move" << endl;
    return 2;
  }
  
  if (m.action == "promotion") {
    if (willLeadToCheck(m)) {
      cerr << "This move exposes your king to a check" << endl;
      return 2;
    }

    simulateMove(m, prom);

    m.moved->incrementMoves();
    record.push_back(m);
    nextTurn();

    return checkGameStatus();
  }

  // should never get here
  return 2;
}

void ChessBoard::revertSimulation(const Move &m) {
  if (m.action == "castling") {
    // need to move king back
    addPieceNoIncrement(m.from, m.moved);
    removePiece(m.to);
    // need to move rook back
    Pos RookAfterPos = m.captured->getPos();
    Pos RookBeforePos{RookAfterPos.row, RookAfterPos.col == 3 ? 0 : 7};

    addPieceNoIncrement(RookBeforePos, m.captured);
    removePiece(RookAfterPos);
  }
  else if (m.action == "en-passant") {
    Pos tmp{m.from.row, m.to.col};
    addPieceNoIncrement(tmp, m.captured);
    addPieceNoIncrement(m.from, m.moved);
    removePiece(m.to);
  }
  else if (m.action == "promotion") {
    // remove promoted piece from pieceset and the board
    char owner = getPiece(m.to)->getOwner();
    owner == 'w' ? white.pop_back() : black.pop_back();
    removePiece(m.to);
    // add pawn back
    addPieceNoIncrement(m.from, m.moved);
    // also check if there was a capture
    if (m.captured != nullptr) addPieceNoIncrement(m.to, m.captured);
  }
  else if (m.action == "capture") {
    addPieceNoIncrement(m.from, m.moved);
    addPieceNoIncrement(m.to, m.captured);
  }
  else if (m.action == "move") {
    addPieceNoIncrement(m.from, m.moved);
    removePiece(m.to);
  }
}

void ChessBoard::undoMove() {
  if (record.empty()) return;
  const Move m = record.back();

  revertSimulation(m);
  if (m.action == "castling") m.captured->decrementMoves();
  m.moved->decrementMoves();

  record.pop_back();
  prevTurn();
}

Piece *ChessBoard::getKing(bool isWhiteTurn) {
  return isWhiteTurn ? white.front().get() : black.front().get();
}

// gets all valid moves for white is isWhiteTurn is true, 
// and for black otherwise
vector<Move> ChessBoard::getAllValidMoves(bool isWhiteTurn) {
  vector<unique_ptr<Piece>> &pieceSet = isWhiteTurn ? white : black;
  int pieceSetSize = pieceSet.size();
  vector<Move> validMoves;
  Pos removed{-1, -1};
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      // fuck the iterator invalidation
      for (int idx = 0; idx < pieceSetSize; ++idx) {
        const unique_ptr<Piece> &p = pieceSet.at(idx);
        Pos from = p->getPos();
        if (!(from == removed)) {
          Pos to{row, col};
          Move m = isValidMove(from, to);
          if (m.action != "illegal") {
            if (!willLeadToCheck(m)) validMoves.push_back(m);
          }
        }
      }
    }
  }
  validMoves.shrink_to_fit();
  return validMoves;
}

// finds all possible opponents' move to a position
// invariant: p must be within the chessboard
vector<Move> ChessBoard::getThreats(const Pos &p, bool isWhiteTurn) {
  Pos removed{-1, -1};
  vector<Move> threats;
  const vector<unique_ptr<Piece>> &pieceSet = isWhiteTurn ? black : white;
  for (auto &piece : pieceSet) {
    if (!(piece->getPos() == removed)) {
      Move m = isValidMove(piece->getPos(), p);
      if (m.action != "illegal") {
        threats.push_back(m);
      }
    }
  }
  return threats;
}

// finds all possible allys' move to a position
// invariant: p must be within the chessboard and
//            p must not be occupied by the same color
vector<Move> ChessBoard::getCovers(const Pos &p, bool isWhiteTurn) {
  Pos removed{-1, -1};
  vector<Move> covers;
  const vector<unique_ptr<Piece>> &pieceSet = isWhiteTurn ? white : black;
  for (auto &piece : pieceSet) {
    if (!(piece->getPos() == removed)) {
      Move m = isValidMove(piece->getPos(), p);
      if (m.action != "illegal") {
        covers.push_back(m);
      }
    }
  }
  return covers;
}

// finds all possible Moves for a piece
vector<Move> ChessBoard::getMoves(Piece *p) {
  vector<Move> moves;
  Pos removed{-1, -1};
  if (p == nullptr || p->getPos() == removed) return moves;
  
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      Pos to{row, col};
      Move m = isValidMove(p->getPos(), to);
      if (m.action != "illegal") {
        if (!willLeadToCheck(m)) moves.push_back(m);
      }
    }
  }
  moves.shrink_to_fit();
  return moves;
}

// checks if white has no legal moves available if isWhiteTurn is true
// and if black has no legal moves available if isWhiteTurn is false
bool ChessBoard::isStalemate(bool isWhiteTurn) {
  vector<Move> possibleMoves = getAllValidMoves(isWhiteTurn);
  for (auto &m : possibleMoves) {
    simulateMove(m);

    if (!isCheck(isWhiteTurn)) {
      revertSimulation(m);
      return false;
    }

    revertSimulation(m);
  }
  return true;
}

// returns if white is in checkmate if isWhiteTurn is true
// and if black is in checkmate if isWhiteTurn is false
bool ChessBoard::isCheckmate(bool isWhiteTurn) { 
  Piece *kptr = getKing(isWhiteTurn);
  Pos k = kptr->getPos();

  vector<Move> kingMoves = getMoves(kptr);

  for (auto &km : kingMoves) {
    simulateMove(km);

    if (getThreats(km.to, isWhiteTurn).empty()) {
      revertSimulation(km);
      return false;
    }

    revertSimulation(km);
  }

  // or other ally can block for the king
  // 1. get squares that are movable
  vector<Move> possibleMoves = getAllValidMoves(isWhiteTurn);
  vector<Move> threats = getThreats(k, isWhiteTurn);
  // 2. check if one of those blocks every attacker
  for (auto &m : possibleMoves) {
    if (m.moved->getPiece() != 'k') {
      simulateMove(m);
      
      bool blockEveryThreat = true;
      for (auto &t : threats) {
        if (isValidMove(t.from, k).action != "illegal") {
          blockEveryThreat = false;
          break;
        }
      }

      revertSimulation(m);
      if (blockEveryThreat == true) return false; 
    }
  }

  // or other ally can take the attacker
  // 1. get which pieces are attacking the king
  // 2. if there is only one attacker, and that piece's square is in one 
  // of your piece's attacked square, 
  if (threats.size() == 1) {
    for (auto &m : possibleMoves) {
      if (m.moved->getPiece() != 'k') {
        if (m.to == threats.front().from) return false;
      }
    }
  }

  return true;
}

// returns if white is in check if isWhiteTurn is true
// and if black is in check if isWhiteTurn is false
bool ChessBoard::isCheck(bool isWhiteTurn) { 
  Pos k = getKing(isWhiteTurn)->getPos();
  vector<Move> threats = getThreats(k, isWhiteTurn);
  if (!threats.empty()) return true;
  return false;
}

// assume that m is a valid move
bool ChessBoard::willLeadToCheck(const Move &m) {
  // 1. simulate the move
  simulateMove(m);
  // 2. see if it gives a check
  bool check = isCheck(isWhiteTurn());
  // 3. undo the simulation
  revertSimulation(m);
  // 4. return the result
  return check; 
}

bool ChessBoard::canCastle(Piece *king, Piece *rook) { 
  if (rook == nullptr || king == nullptr) return false;
  if (rook->didMove() || king->didMove()) return false;
  if (rook->getPiece() != 'r') return false;

  int rookCol = rook->getPos().col;
  int kingCol = king->getPos().col;

  int low = rookCol > kingCol ? kingCol : rookCol;
  int high = rookCol > kingCol ? rookCol : kingCol;
  int diff = high - low;

  Pos tmp;
  tmp.row = king->getPos().row;

  // check if there isn't any piece in between
  if ((rookCol == 0) || (rookCol == 7)) {
    for (int i = 1; i < diff; ++i) {
      tmp.col = low + i;
      if (getPiece(tmp) != nullptr) return false;
    }
  }

  // check if king is not in check at start, end, or in between
  if (rookCol == 0) {
   for (int j = 2; j <= kingCol; ++j) {
     tmp.col = j;
     if (!getThreats(tmp, isWhiteTurn()).empty()) return false;
   }
   return true;
  }
  if (rookCol == 7) {
   for (int k = kingCol; k <= 6; ++k) {
     tmp.col = k;
     if (!getThreats(tmp, isWhiteTurn()).empty()) return false;
   }
   return true;
  }

  return false;
}

bool ChessBoard::canEnPassant(Piece *p1, const Pos& from, const Pos& to) {
  // No point checking when no moves made
  //if (record.size() == 0)
  //  return 0;

  if (p1->getOwner() == 'w') {
    for (auto &&piece : black) {
      // Same row and are neighbours by col
      if (from.row == 3 && piece->getPos().row == 3 && abs(piece->getPos().col - from.col) == 1) {
        if (record.back().to == piece->getPos()) {
          return to.row == 2 && to.col == piece->getPos().col;
        }
      }
    }
  }
  else {
    for (auto &&piece : white) {
      // Same row and are neighbours by col
      if (from.row == 4 && piece->getPos().row == 4 && abs(piece->getPos().col - from.col) == 1) {
        if (record.back().to == piece->getPos()) {
          return to.row == 5 && to.col == piece->getPos().col;
        }
      }
    }
  }
  return false;
}

bool ChessBoard::canSetup() {
  clearEmptyPieces();
  int whiteKingCount = 0;
  int blackKingCount = 0;
  // Counts the number of kings
  for (auto &&piece : white) {
    if (piece->getPiece() == 'k') ++whiteKingCount;

    // Checks if pawns are on first or last row
    if (piece->getPiece() == 'p') {
      int row = piece->getPos().row;
      if (row == 0 || row == 7) return false;
    }
  }
  for (auto &&piece : black) {
    if (piece->getPiece() == 'k') ++blackKingCount;

    // Checks if pawns are on first or last row
    if (piece->getPiece() == 'p') {
      int row = piece->getPos().row;
      if (row == 0 || row == 7) return false;
    }
  }

  if (whiteKingCount != 1 || blackKingCount != 1) return false;

  // Checks if white king is in check
  if (isCheck(true)) return false;
  // Checks if black king is in check
  if (isCheck(false)) return false;

  return true;
}

bool ChessBoard::isWithin(const Pos &pos) { return pos.row < 8 && pos.row >= 0 && pos.col < 8 && pos.col >= 0; }
bool ChessBoard::isEnemy(char owner, const Pos &pos) {
  Piece * piece = getPiece(pos);
  return piece != nullptr && piece->getOwner() != owner;
}
bool ChessBoard::isFriendly(char owner, const Pos &pos) {
  Piece* piece = getPiece(pos);
  return piece != nullptr && piece->getOwner() == owner;
}
bool ChessBoard::isObstacle(const Pos &pos) { return getPiece(pos) != nullptr; }

void ChessBoard::promote(const Pos &from, const Pos &to, char prom) {
  char ownerFromInput;
  // Black piece
  if (prom == tolower(prom)) {
    ownerFromInput = 'b';
  }
  // White piece
  else if (prom == toupper(prom)) {
    ownerFromInput = 'w';
    prom = tolower(prom);
  } 
  // Handles invalid piece
  else throw runtime_error("Invalid Piece!");

  char owner = getPiece(from)->getOwner();
  if (ownerFromInput != owner) throw runtime_error("Cannot Promote to an opponent piece!");
    
  vector<unique_ptr<Piece>> &pieceSet = owner == 'w' ? white : black;
  switch (prom) {
    case 'q':
      pieceSet.push_back(make_unique<Queen>(owner, to.row, to.col));
      break;
    case 'r':
      pieceSet.push_back(make_unique<Rook>(owner, to.row, to.col));
      break;
    case 'b':
      pieceSet.push_back(make_unique<Bishop>(owner, to.row, to.col));
      break;
    case 'n':
      pieceSet.push_back(make_unique<Knight>(owner, to.row, to.col));
      break;
    default:
      throw runtime_error("Invalid Piece!");
  }
  removePiece(from, true);
  if (getPiece(to) != nullptr) removePiece(to, true); 
  addPiece(to, pieceSet.back().get());
}

// Prints final score
void ChessBoard::printScore() { 
  cout << "Final Score:" << endl;
  cout << "White: " << score[0] << endl; 
  cout << "Black: " << score[1] << endl;
  cout << endl;
}

void ChessBoard::render() { updateObservers(); }

int ChessBoard::getTurn() { return turn; }

void ChessBoard::setTurn(int turn) { this->turn = turn; }

void ChessBoard::nextTurn() { ++turn; }

void ChessBoard::prevTurn() { --turn; }

bool ChessBoard::isWhiteTurn() { return turn % 2 == 0; }

// white goes first, then take turn
bool ChessBoard::isValidTurn(const Pos &from) {
  Piece *piece = getPiece(from);
  if (piece == nullptr) return false;
  char whoseTurn = isWhiteTurn() ? 'w' : 'b';
  if (piece->getOwner() != whoseTurn) return false;
  return true;
}

void ChessBoard::resign() {
  if (isWhiteTurn()) ++score[1];
  else ++score[0];
}
