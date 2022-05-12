#include "position.h"

Pos::Pos(int row, int col) : row{row}, col{col} {}

std::operator==(const Pos& n) const {
    return row == n.row && col == n.col;
} 