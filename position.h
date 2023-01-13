#ifndef __POSITION_H__
#define __POSITION_H__

#include <iostream>
#include <string>

struct Pos {
    int row;
    int col;
    Pos();
    Pos(const std::string& pos);
    Pos(int row, int col);
    bool operator==(const Pos& n) const;
};

std::ostream &operator<<(std::ostream &out, const Pos &);

#endif