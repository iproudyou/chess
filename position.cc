#include "position.h"

Pos::Pos() : row{-1}, col{-1} {}

Pos::Pos(int row, int col) : row{row}, col{col} {}

Pos::Pos(const std::string& pos) : row{8 - pos[1] + '0'}, col{pos[0] - 'a'} {}

bool Pos::operator==(const Pos& n) const {
    return row == n.row && col == n.col;
} 

std::ostream& operator<<(std::ostream& out, const Pos & position) {
    out << "(row,col) => (" << position.row << "," << position.col << ")";
    return out;
}