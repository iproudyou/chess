#include "level.h"
#include "piece.h"
#include "chessBoard.h"
#include "chessMove.h"
#include <memory>
#include <iostream>

using namespace std;

Level::Level(ChessBoard *b) : 
    b{b}, 
    weightMap {{'k',9000}, {'q',900}, {'r',500}, {'b',350}, {'n',320}, {'p',100}}, // K, Q, R, B, K, P
    pawnTable {
        0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
        5,  5, 10, 27, 27, 10,  5,  5,
        0,  0,  0, 25, 25,  0,  0,  0,
        5, -5,-10,  0,  0,-10, -5,  5,
        5, 10, 10,-25,-25, 10, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0
    },
    knightTable {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-20,-30,-30,-20,-40,-50,
    },
    bishopTable {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -20,-10,-40,-10,-10,-40,-10,-20,
    },
    kingTable {
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -20, -30, -30, -40, -40, -30, -30, -20,
        -10, -20, -20, -20, -20, -20, -20, -10, 
         20,  20,   0,   0,   0,   0,  20,  20,
         20,  30,  10,   0,   0,  10,  30,  20
    } {}
Level::~Level() {}

bool Level::leadsToCheck(const Move& move, bool isWhite) {
    return move.moved->isValidMove(b, b->getKing(!isWhite)->getPos());
}

bool Level::preferCaptureCheck(Move& aiMove, const std::vector<Move>& possibleMoves, bool isWhite) {
    bool moveFound = false;
    for (auto & move : possibleMoves) {
        // Checks if a piece is captured
        if (move.action == "promotion" || move.action == "en-passant" || move.action == "capture") {
            if (move.captured != nullptr) {
                aiMove = move;
                moveFound = true;
                break;
            }
        } else if (move.action == "move") {
            b->simulateMove(move);
            if (leadsToCheck(move, isWhite)) {
                aiMove = move;
                moveFound = true;
                b->revertSimulation(move);
                break;
            }
            b->revertSimulation(move);
        }
    }
    return moveFound;
}

Move Level::generateRandomMove(const std::vector<Move>& possibleMoves) {
    time_t t;
    int size = possibleMoves.size();
    srand(time(&t));
    int randomInd = rand() % size;
    return possibleMoves.at(randomInd);
}

int Level::evaluatePiece(Piece* piece) {
    Pos current = piece->getPos();
    char pieceName = piece->getPiece();
    char owner = piece->getOwner();

    int mapIndex = (current.row * 8) + current.col;
    if (owner == 'b') {
        mapIndex = 63 - mapIndex;
    }

    // Computes a piece's value 
    int pieceScore = weightMap.at(pieceName);
    switch(pieceName) {
        case 'p':
            pieceScore += pawnTable[mapIndex];
            break;
        case 'n':
            pieceScore += knightTable[mapIndex];
            break;
        case 'b':
            pieceScore += bishopTable[mapIndex];
            break;
        case 'k':
            pieceScore += kingTable[mapIndex];
            break;
    }

    return pieceScore;
}

int Level::evaluateBoard(char maxColour) {
    Pos tmp {};
    int whiteCurState = 0;
    int blackCurState = 0;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            tmp.row = i;
            tmp.col = j;
            Piece *p = b->getPiece(tmp);
            if (p != nullptr) {
                if (p->getOwner() == 'w') 
                    whiteCurState += evaluatePiece(p);
                else 
                    blackCurState += evaluatePiece(p);
            }
        }
    }

    return maxColour == 'w' ? (whiteCurState - blackCurState) : (blackCurState - whiteCurState);
}