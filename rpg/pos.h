#ifndef POS_H
#define POS_H

class Pos {
public:
    int row;
    int col;
    
    Pos();
    Pos(Pos &other);
    Pos(int row, int col);
};

#endif
