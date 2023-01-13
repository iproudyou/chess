#ifndef __CHESSBOARD_H__
#define __CHESSBOARD_H__

#include <memory>
#include <vector>
#include "position.h"
#include "chessSubject.h"
#include "piece.h"

struct Move;

class ChessBoard : public ChessSubject {
  int turn; // Assumes white goes first, unless specified otherwise in setup
  double score [2];
  Piece *board[8][8];
  std::vector<std::unique_ptr<Piece>> white;
  std::vector<std::unique_ptr<Piece>> black;
  std::vector<Move> record;
  void createPiece(char& owner, char c, int row, int col,std::unique_ptr<Piece>& p);
  void clearEmptyPieces();

  public:
    ChessBoard();
    ~ChessBoard();

    // Initializes to a normal board setup
    void initializeBoard();

    // for setup: adds a piece to white/black and on the board
    void addPiece(const Pos &pos, char piece);
    // for setup: removes a piece
    void removeSetupPiece(const Pos pos);

    // used to replay the game
    void clearAllBoardState();

    // Adds a piece to pos on the board
    void addPiece(const Pos &pos, Piece *piece);
    // Adds a piece to p on the board without incrementing totalMoves count in Piece
    void addPieceNoIncrement(const Pos &pos, Piece *piece);
    // Removes a piece from the board
    // If capture is true, then it set the pos to {-1, -1}
    void removePiece(const Pos pos, bool capture = false);
    // Gets a pointer to a piece at pos
    Piece *getPiece(const Pos &pos);

    // checks if from to to is a valid move
    // this does not check if the move will lead to a check of its own king
    // action from returning move will be illegal/en-passant/promotion/castling/capture/move
    Move isValidMove(const Pos &from, const Pos &to);
    // simulates a move without incrementing totalMoves count in Piece
    void simulateMove(const Move &m, char prom = ' ');
    // checks for check, checkmate, stalemate and increment score accordingly
    int checkGameStatus();
    // Handles resignation from player
    void resign();
    // Moves piece to pos
    // 1 = legal move, 2 = illegal move, 3 = checkmate, 4 = stalemate
    int move(const Pos &from, const Pos &to);
    // handles promotion only
    int move(const Pos &from, const Pos &to, char prom);
    // Promotes pawn to a specified piece
    void promote(const Pos &from, const Pos &to, char prom);
    // reverts the previously performed simulation
    void revertSimulation(const Move &m);
    // Undo a single move
    void undoMove();

    // Gets all the valid moves (useful for computer) 
    // Perform deep copy to avoid dependency issues
    std::vector<Move> getAllValidMoves(bool isWhiteTurn);

    // finds all possible opponents move to a position
    std::vector<Move> getThreats(const Pos &p, bool isWhiteTurn);
    // finds all possible allys move to a position
    std::vector<Move> getCovers(const Pos &p, bool isWhiteTurn);
    // finds all possible Moves for a piece
    std::vector<Move> getMoves(Piece *p);
    // Gets the specified king
    Piece *getKing(bool isWhiteTurn);

    // Checks for stalemate
    bool isStalemate(bool isWhiteTurn);
    // Checks for checkmate
    bool isCheckmate(bool isWhiteTurn);
    // Checks for check
    bool isCheck(bool isWhiteTurn);
    // check if a move will result in a check of the player's own king
    // assume that m is a valid move
    bool willLeadToCheck(const Move &m);

    // Checks if pos is within boundary
    bool isWithin(const Pos& pos);
    // Checks if there is an enemy at pos
    bool isEnemy(char owner, const Pos& pos);
    // Checks if there is a friendly piece at pos
    bool isFriendly(char owner, const Pos& pos);
    // Checks if there is an obstacle at pos
    bool isObstacle(const Pos& pos);

    // Checks for castling condition
    bool canCastle(Piece *king, Piece *rook);
    // Checks for en-passant condition
    bool canEnPassant(Piece *p1, const Pos& from, const Pos& to);
    // Checks if the setup is valid
    bool canSetup();
    
    // Prints white and black's score
    void printScore();
    // Updates observers
    void render();

    // Gets the current turn
    int getTurn();
    // Sets the turn 
    void setTurn(int turn);
    // Moves to the next turn
    void nextTurn();
    // Moves to the previous turn
    void prevTurn();

    bool isWhiteTurn();
    bool isValidTurn(const Pos &from);
};

#endif


