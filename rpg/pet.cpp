#include <iostream>

using namespace std;

#include "main.h"
#include "prop.h"
#include "unit.h"
#include "monster.h"
#include "pet.h"
#include "pos.h"
#include "brain.h"
#include "board.h"

//extern Board *board;
//extern int numTicks;
#include "sim.h"
extern Sim *sim;

//------------------------------------------------------------------------------
// class Pet
//------------------------------------------------------------------------------
Pet::Pet() : Unit('p', 0, 0, 0, 0, 0, 0, DIR_NONE, 0, 0, 0, 0) {
    master = NULL;
}

Pet::Pet(char shape, int row, int col, int hp, int maxHp, int mp, int maxMp, int dir, int atk, int def, int gold, int exp, Unit *master, bool timeToReturn)
    : Unit (shape, row, col, hp, maxHp, mp, maxMp, dir, atk, def, gold, exp) {
    this->master = master;
}

void Pet::reset() {
    Unit::reset();
//    master = NULL;
}

Pet::~Pet() {
    // master는 delet하지 말 것.
}

void Pet::setMaster(Unit *master) {
    this->master = master;
}

bool Pet::isPet() {
    return true;
}

bool Pet::isHellDog() {
    return false;
}

void Pet::returnToMaster(Unit *master) {
    
}

bool Pet::inMasterRange() {
    return false;
}

//------------------------------------------------------------------------------
// class HellDog
//------------------------------------------------------------------------------
HellDog::HellDog() : Pet(SHAPE_HELLDOG, 0, 0, HELLDOG_DEFAULT_MAX_HP, HELLDOG_DEFAULT_MAX_HP, 0, 0, DIR_NONE, HELLDOG_DEFAULT_ATK, 0, 0, HELLDOG_DEFAULT_EXP, NULL, false) {
    range = HELLDOG_DEFAULT_RANGE;
    countToReturn = HELLDOG_RETURN_DELAY;
    brain = new BrainIQ120(this);
}

HellDog::HellDog(Unit *master) : Pet(SHAPE_HELLDOG, 0, 0, HELLDOG_DEFAULT_MAX_HP, HELLDOG_DEFAULT_MAX_HP, 0, 0, DIR_NONE, HELLDOG_DEFAULT_ATK, 0, 0, HELLDOG_DEFAULT_EXP, master, false) {
    range = HELLDOG_DEFAULT_RANGE;
    countToReturn = HELLDOG_RETURN_DELAY;
    brain = new BrainIQ120(this);
}

void HellDog::reset() {
    Pet::reset();
//    range = HELLDOG_DEFAULT_RANGE;
//    countToReturn = HELLDOG_RETURN_DELAY;
}

HellDog::~HellDog() {
}

bool HellDog::isHellDog() {
    return true;
}

void HellDog::returnToMaster(Unit *master) {
    Pos masterPos(master->getRow(), master->getCol());
    brain->findPath(masterPos);
    return;
}

bool HellDog::inMasterRange() {
    if (this->getRow() >= master->getRow() - HELLDOG_MASTER_RANGE &&
        this->getRow() <= master->getRow() + HELLDOG_MASTER_RANGE &&
        this->getCol() >= master->getCol() - HELLDOG_MASTER_RANGE &&
        this->getCol() <= master->getCol() + HELLDOG_MASTER_RANGE) {
        return true;
    }
    
    return false;
}

void HellDog::move(int dir) {
//    cout << "HellDog::move(): started" << endl;
    if (isDead()) {
        return;
    }
    
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // 따라갈 path가 있을 경우, 일단 무조건 그 path를 따라간다. 단 뭔가 막히면 그 즉시 path해제.
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    
    // IQ 120/IQ 150등에서 찾은 path가 있다면 그것을 다 따라가기 전까지는 계속 따라간다.
    if (brain->isPathFound()) {
        Pos *nextPos = brain->getNextPos();
        
        if (!sim->board->isBlocked(nextPos->row, nextPos->col)) {
            sim->board->setUnit(row, col, NULL);
            sim->board->setUnit(nextPos->row, nextPos->col, this); // 이 코드에서 boss의 row, col도 바뀌므로 주의하자.
            brain->setNextPos();
        }
        else {
            brain->reset();
        }
        // 만약 막혔다면 한번 또는 계속 기다린다. monster등에 의해 path가 잠시 막힐 수 있다.
        return;
    }

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // HELLDOG_RETURN_DELAY가 지났을 경우, master가 masterRange밖에 있다면, 일단 master에게 돌아가도록 해야 함.
    // monster와 싸우던 중에라도 다시 돌아가야 함.
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    countToReturn--;
    
    if (countToReturn <= 0) {
        if (!inMasterRange()) {
            returnToMaster(master);
            return;
        }
        countToReturn = HELLDOG_RETURN_DELAY;
    }
    
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // range안에 monster가 있다면 무조건 공격.
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    
    Pos monsPos;
    bool monsFound = brain->findMons(monsPos, range);
    
    //    cout << "HellDog::move(): hello1" << endl;
    if (monsFound) {
        //        cout << "HellDog::move(): hello2" << endl;
        brain->findPath(monsPos);
    }
    
    Monster *monsAdjacent = NULL;
    
    if ((monsAdjacent = getMons(row-1, col  )) != NULL ||
        (monsAdjacent = getMons(row,   col+1)) != NULL ||
        (monsAdjacent = getMons(row+1, col  )) != NULL ||
        (monsAdjacent = getMons(row  , col-1)) != NULL) {
        monsAdjacent->decHp(atk);
    }
    
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // Hero에게 돌아가기(range안에 공격할 monster가 없을 경우..)
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    
    if (!monsFound && master != NULL) {
        Pos masterPos(master->getRow(), master->getCol());
        
        brain->findPath(masterPos);
        if (brain->isPathFound()) {
            return;
        }
    }
    
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // 일반 move
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    bool moved = false;
    
    int prevRow = row;
    int prevCol = col;
    
    int nextRow = row;
    int nextCol = col;
    
    if (dir == DIR_N) {
        nextRow--;
    }
    else if (dir == DIR_E) {
        nextCol++;
    }
    else if (dir == DIR_S) {
        nextRow++;
    }
    else if (dir == DIR_W) {
        nextCol--;
    }
    
    if (!sim->board->isOutOfBoundary(nextRow, nextCol) &&
        sim->board->getUnit(nextRow, nextCol) == NULL &&
        (sim->board->getProp(nextRow, nextCol) == NULL || sim->board->getProp(nextRow, nextCol)->isClimbable())) {
        sim->board->setUnit(prevRow, prevCol, NULL);
        sim->board->setUnit(nextRow, nextCol, this);
        moved = true;
    }
}

Monster *HellDog::getMons(int row, int col) {
    if (!sim->board->isOutOfBoundary(row, col) &&
        sim->board->getUnit(row, col) != NULL &&
        sim->board->getUnit(row, col)->isMonster()) {
        return (Monster *)sim->board->getUnit(row, col);
    }
    else {
        return NULL;
    }
}

void HellDog::interact(Unit *unit) {
}
