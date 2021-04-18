#include <iostream>

using namespace std;

#include "main.h"
#include "unit.h"
#include "prop.h"
#include "hero.h"
#include "inventory.h"
#include "board.h"
#include "ridable.h"
#include "brain.h"

//extern Board *board;
#include "sim.h"
extern Sim *sim;

Ridable::Ridable() : Unit('_', 0, 0, RIDABLE_DEFAULT_MAX_HP, RIDABLE_DEFAULT_MAX_HP, 0, 0, DIR_NONE, RIDABLE_DEFAULT_ATK, 0, 0, 0) {
    rider = NULL;
}

Ridable::Ridable(char shape, int row, int col, int hp, int maxHp, int mp, int maxMp, int dir, int atk, int def, int gold, int exp)
    : Unit(shape, row, col, hp, maxHp, mp, maxMp, dir, atk, def, gold, exp) {
    rider = NULL;
}

Ridable::Ridable(Ridable &other) : Unit(other) {
    rider = NULL; // rider를 copy하지 않는 이유는, 한명의 rider가 두대의 car를 동시에 몰 수 없으므로, 당연히 copy하면 안 된다.
}

void Ridable::reset() {
    Unit::reset();
    rider = NULL; // 혹시 mon이 horse를 타고 있으면, mon이 memory leak될 수도 있다.
}

Ridable::~Ridable() {
    if (rider != NULL) {
        delete rider;
        rider = NULL;
    } //이거는 tile destructor에서 해주지 않나?
}

bool Ridable::isRidable() {
    return true;
}

bool Ridable::isCar() {
    return false;
}

bool Ridable::isOccupied() {
    return rider != NULL;
}

Unit *Ridable::getRider() {
    return rider;
}

void Ridable::setRider(Unit *rider) {
    this->rider = rider;
}

bool Ridable::kickOut() {
    if (rider != NULL && rider->getOff()) {
        rider = NULL;
        return true;
    }
    
    return false;
}

Car::Car() : Ridable(SHAPE_CAR, 0, 0, CAR_DEFAULT_MAX_HP, CAR_DEFAULT_MAX_HP, 0, 0, DIR_NONE, CAR_DEFAULT_ATK, 0, 0, 0) {
//    this->unit = NULL;
}

Car::Car(Car &other) : Ridable(other) {
//    this->unit = other.unit;
}

Car::Car(char shape, int row, int col, int hp, int maxHp, int mp, int maxMp, int dir, int atk, int def, int gold, int exp)
    : Ridable(shape, row, col, hp, maxHp, mp, maxMp, dir, atk, def, gold, exp) {
    //    this->unit = unit;
}

void Car::reset() {
    Ridable::reset();
}

Car::~Car() {
//    if(unit != NULL) {
//        delete unit;
//        unit = NULL;
//    }
}

//Unit *Car::getUnit() {
//    return unit;
//}
//
//void Car::setUnit(Unit *unit) {
//    this->unit = unit;
//}

bool Car::isCar() {
    return true;
}

void Car::printStat() {
    Unit::printStat();
    cout << endl;
}

void Car::move(int dir) {
    // 차는 받아온 dir를 사용하지 않는다.
    // 대신 rider의 dir를 사용한다.
    
    if (isDead() || rider == NULL) {
        return;
    }
    this->dir = rider->getDir();
    
    bool moved = false;
    
    int prevRow = row;
    int prevCol = col;
    
    int nextRow = row;
    int nextCol = col;
    
    if (rider->getDir() == DIR_N) {
        nextRow--;
    }
    else if (rider->getDir() == DIR_E) {
        nextCol++;
    }
    else if (rider->getDir() == DIR_S) {
        nextRow++;
    }
    else if (rider->getDir() == DIR_W) {
        nextCol--;
    }
    else {
        return;
    }
    
    if (!sim->board->isOutOfBoundary(nextRow, nextCol) &&
        sim->board->getUnit(nextRow, nextCol) == NULL &&
        (sim->board->getProp(nextRow, nextCol) == NULL || sim->board->getProp(nextRow, nextCol)->isClimbable())) {
        sim->board->setUnit(prevRow, prevCol, NULL);
        sim->board->setUnit(nextRow, nextCol, this);
        moved = true;
        
//        setDir(dir);
        
        rider->setRow(row);
        rider->setCol(col);
//        rider->setDir(dir);
    }
}


void Car::interact(Unit *unit) {
    if (unit == NULL) {
        cout << "Ridable::interact(): error: unit == NULL" << endl;
        exit(1);
    }
    setRider(unit);
    sim->board->setUnit(unit->getRow(), unit->getCol(), NULL);
    unit->setRow(this->getRow());
    unit->setCol(this->getCol());
    unit->setRide(this);
}

