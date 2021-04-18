#include "pos.h"

Pos::Pos() {
    row = -1;
    col = -1;
}

Pos::Pos(Pos &other) {
    row = other.row;
    col = other.col;
}

Pos::Pos(int row, int col) {
    this->row = row;
    this->col = col;
}
