#include <iostream>

using namespace std;

#include "main.h"
#include "item.h"
#include "prop.h"
#include "unit.h"
#include "tile.h"
#include "board.h"
#include "magic.h"
#include "sim.h"
#include "pos.h"

//extern sim->board *sim->board;
//extern int numTicks;
extern Sim *sim;

Tile::Tile() {
    throwing = NULL;
    fx = FX_NONE;

    unit = NULL;
    item = NULL;
    prop = NULL;
    
    //--------------------------------------------------------------------------
    // for A*
    //--------------------------------------------------------------------------
    resetAStar();
    row = -1;
    col = -1;
}

Tile::Tile(int row, int col) {
    throwing = NULL;
    fx = FX_NONE;
    
    unit = NULL;
    item = NULL;
    prop = NULL;

    //--------------------------------------------------------------------------
    // for A*
    //--------------------------------------------------------------------------
    resetAStar();
    this->row = row;
    this->col = col;
}

Tile::~Tile() {
    if (throwing != NULL) {
        delete throwing;
        throwing = NULL;
    }
    if (unit != NULL) {
        delete unit;
        unit = NULL;
    }
    if (item != NULL) {
        delete item;
        item = NULL;
    }
    if (prop != NULL) {
        delete prop;
        prop = NULL;
    }
}

void Tile::print() {
//    if (magic != NULL) {
//        magic->print();
//    }
//    
//    else if (throwing != NULL) {
    if (throwing != NULL) {
        ThrowableItem *throwableItem = (ThrowableItem *)throwing;
        if (throwableItem->getNumTicksWhenMoved() == sim->time) {
            return;
        }
        throwing->print();
        
//        cout << "Tile::print(): throwing->getRow(): " << throwing->getRow() << " | throwing->getCol(): " << throwing->getCol() << endl;

        if (throwableItem->getCurFlyingDist() >= throwableItem->getRange()) {
            // 만약 이 자리의 item에 떨어뜨려야 하는데, 뭔가 있다면, 그 아이템을 아예 지워버리는
            // 쪽으로 하자. 하지만 진짜 게임에서는 그 근처에 떨어뜨리거나, 그 자리에 쌓이도록 해야 한다.
            if (item != NULL) {
                delete item;
            }
            throwableItem->resetCurFlyingDist(); // throwableItem은 아래의 throwing과 같은 것임에 주의하자.
            item = throwing;
            throwing = NULL;
            
//            cout << "Tile::print(): arrived: " << throwableItem->getCurFlyingDist() << endl;
//            exit(1);
        }
        // 계속 날라가는 방향으로 날라가게 하는데, 만약 그 자리에 prop이 있거나, unit이 있으면 처리.
        else {
            int curDir = throwing->getDir();
            int nextRow = throwing->getRow();
            int nextCol = throwing->getCol();
            if (curDir == DIR_N) {
                nextRow--;
            }
            else if (curDir == DIR_E) {
                nextCol++;
            }
            else if (curDir == DIR_S) {
                nextRow++;
            }
            else if (curDir == DIR_W) {
                nextCol--;
            }
            
            if (sim->board->getProp(nextRow, nextCol) != NULL) {
                // 만약 이 자리의 item에 떨어뜨려야 하는데, 뭔가 있다면, 그 아이템을 아예 지워버리는
                // 쪽으로 하자. 하지만 진짜 게임에서는 그 근처에 떨어뜨리거나, 그 자리에 쌓이도록 해야 한다.
                if (item != NULL) {
                    delete item;
                }
                throwableItem->resetCurFlyingDist(); // throwableItem은 아래의 throwing과 같은 것임에 주의하자.
                item = throwing;
                throwing = NULL;
            }
            else if (sim->board->getUnit(nextRow, nextCol) != NULL &&
                     sim->board->getUnit(nextRow, nextCol)->isMonster()) {
                sim->board->getUnit(nextRow, nextCol)->decHp(throwableItem->getAtk());
                // dagger의 disposable이 아니기 때문에 monster들의 inventory에 넣어줘서 나중에 hero가 looting할 수 있게 해줘야 하지만, 여기에서는 그냥 지우자.
                delete throwing;
                throwing = NULL;
            }
            else {
                throwableItem->incCurFlyingDist();
                throwableItem->setNumTicksWhenMoved(sim->time);
//                cout << "Tile::print(): flying: " << throwableItem->getCurFlyingDist() << endl;
//                cout << "Tile::print(): nextRow: " << nextRow << " | nextCol: " << nextCol << endl;

//                exit(1);
                sim->board->setThrowing(nextRow, nextCol, throwing);
                throwing = NULL;
            }
        }
    }
    else if (fx != FX_NONE) {
        cout << fx;
    }
    else if (unit != NULL) {
        unit->print();
    }
    else if (item != NULL) {
        item->print();
    }
    else if (prop != NULL) {
        prop->print();
    }
    else {
        cout << SHAPE_EMPTY;
    }
}

char Tile::getFx() {
    return fx;
}

void Tile::setFx(char fx) {
    this->fx = fx;
}

void Tile::clearFx() {
    fx = FX_NONE;
}

//Magic *Tile::getMagic() {
//    return magic;
//}
//
//void Tile::setMagic(Magic *magic) {
//    this->magic = magic;
//}

Item *Tile::getThrowing() {
    return throwing;
}

void Tile::setThrowing(Item *throwing) {
    this->throwing = throwing;
}

void Tile::clearThrowing() {
    throwing = THROWING_NONE;
}

Unit *Tile::getUnit() {
    return unit;
}

void Tile::setUnit(Unit *unit) {
    this->unit = unit;
}

Item *Tile::getItem() {
    return item;
}

void Tile::setItem(Item *item) {
    this->item = item;
}

Prop *Tile::getProp() {
    return prop;
}

void Tile::setProp(Prop *prop) {
    this->prop = prop;
}

//--------------------------------------------------------------------------
// A*용 변수들과 functions
//--------------------------------------------------------------------------
//int row; // me
//int col; // me
//
//int parentRow;
//int parentCol;
//
//double f;
//double g;
//double h;

void Tile::resetAStar() {
    parent = NULL;
    
    open = false;
    closed = false;
    
    f = 0.0;
    g = 0.0;
    h = 0.0;
}

double Tile::heuristicCostEstimate(Tile *goal) {
    return (double)abs(goal->row - row) + (double)abs(goal->col - col);
}

void Tile::calcFGH(Tile *goal) { // me <-> goal
    g = 1.0;
    h = heuristicCostEstimate(goal);
    f = g + h;
}
