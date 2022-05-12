#ifndef __POSITION_H__
#define __POSITION_H__

struct Pos {
    int row;
    int col;
    Pos(int row, int col);
    bool operator==(const Pos& n) const;
};

#endif