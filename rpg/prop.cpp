#include <iostream>

using namespace std;

#include "main.h"
#include "unit.h"
#include "prop.h"
#include "board.h"

//extern Board *board;
#include "sim.h"
extern Sim *sim;

//------------------------------------------------------------------------------
// class Prop
//------------------------------------------------------------------------------

Prop::Prop() {
    shape = 'P';
}

Prop::Prop(char shape) {
    this->shape = shape;
}

void Prop::print() {
    cout << shape;
}

int Prop::getRow() {
    return row;
}
void Prop::setRow(int row) {
    this->row = row;
}

int Prop::getCol() {
    return col;
}

void Prop::setCol(int col) {
    this->col = col;
}

bool Prop::isClimbable() {
    return false;
}

bool Prop::isPortal() {
    return false;
}

//------------------------------------------------------------------------------
// class Tree
//------------------------------------------------------------------------------

Tree::Tree() : Prop(SHAPE_TREE) {}

void Tree::trigger(Unit *unit) {
}

//------------------------------------------------------------------------------
// class Portal
//------------------------------------------------------------------------------

Portal::Portal() : Prop(SHAPE_PORTAL) {}

Portal::Portal(int row, int col) : Prop(SHAPE_PORTAL) {
    this->row = row;
    this->col = col;
}

bool Portal::isClimbable() {
    return true;
}

bool Portal::isPortal() {
    return true;
}

//for random portal
//#define MAX_NUM_ITERATION_TO_FIND_RANDOM_EMPTY_POSITION 10000
//void Portal::trigger(Unit *unit) {
//    int randRow = -1;
//    int randCol = -1;
//    
//    // 랜덤위치를 찾는다.
//    int count = 0;
//    while (true) {
//        randRow = rand() % board->getRowSize();
//        randCol = rand() % board->getColSize();
//        
//        if (count >= MAX_NUM_ITERATION_TO_FIND_RANDOM_EMPTY_POSITION ||
//            (board->getUnit(randRow, randCol) == NULL &&
//             board->getProp(randRow, randCol) == NULL &&
//             board->getItem(randRow, randCol) == NULL)) {
//            break;
//        }
//        
//        count++;
//    }
//    
//    if (randRow != -1) { // found!!
//        board->setUnit(unit->getRow(), unit->getCol(), NULL);
//        board->setUnit(randRow, randCol, unit);
//    }
//    else {
//        cout << "error: Portal::trigger(): can not find empty spot!!" << endl;
//        exit(1);
//    }
//}

void Portal::trigger(Unit *unit) {
//    int randomIndex = rand() % MAX_NUM_PORTALS;
//    while(unit->getRow() == board->getPortalRows(randomIndex) &&
//       unit->getCol() == board->getPortalCols(randomIndex)) {
//        randomIndex = rand() % MAX_NUM_PORTALS;
//    }
    Portal *randomPortal = sim->board->getRandomPortal();
    while(randomPortal->getRow() == unit->getRow() &&
          randomPortal->getCol() == unit->getCol()) {
        randomPortal = sim->board->getRandomPortal();
    }
    
    sim->board->setUnit(unit->getRow(), unit->getCol(), NULL);
    sim->board->setUnit(randomPortal->getRow(), randomPortal->getCol(), unit);
    sim->board->startWave(NULL, NULL, NULL,
                     unit->getRow(), unit->getCol(),
                     4,
                     1,
                     '@',
                     -1);
}
