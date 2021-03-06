#include <iostream>

using namespace std;

#include "main.h"
#include "unit.h"
#include "hero.h"
#include "monster.h"
#include "pos.h"
#include "brain.h"
#include "board.h"
#include "event.h"
#include "priority_queue.h"

//extern Board *board;
#include "sim.h"
extern Sim *sim;

//------------------------------------------------------------------------------
// class Monster
//------------------------------------------------------------------------------

Monster::Monster() : Unit(SHAPE_MONSTER, 0, 0, MONSTER_DEFAULT_MAX_HP, MONSTER_DEFAULT_MAX_HP, 0, 0, DIR_NONE, MONSTER_DEFAULT_ATK, MONSTER_DEFAULT_DEF, MONSTER_DEFAULT_GOLD, MONSTER_DEFAULT_EXP) {
    brain = new Brain(this);
}

Monster::Monster(char shape, int row, int col, int hp, int maxHp, int mp, int maxMp, int dir, int atk, int def, int gold, int exp)
: Unit(shape, row, col, hp, maxHp, mp, maxMp, dir, atk, def, gold, exp) {
    brain = new Brain(this);
}

void Monster::reset() {
    Unit::reset();
}

void Monster::move(int dir) {
    if (isDead()) {
        return;
    }
    
    if (isFrozen() && !isBoss()) {
        return;
    }
    
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
        sim->board->getProp(nextRow, nextCol) == NULL) {
        sim->board->setUnit(prevRow, prevCol, NULL);
        sim->board->setUnit(nextRow, nextCol, this);
        moved = true;
    }
}

bool Monster::isMonster() {
    return true;
}

// battle
void Monster::interact(Unit *unit) {
    if (unit == NULL) {
        cout << "Monster::interact(): error: unit == NULL" << endl;
        exit(1);
    }
    
    if (isDead()) {
        return;
    }
    
    decHp(unit->getAtk()); // hero hits mon
    
    if (!isDead()) {
        unit->decHp(atk);
    }
    else {
        unit->incGold(gold);
        unit->incExp(exp);
        decGold(gold);
        decExp(exp);
    }
}

//------------------------------------------------------------------------------
// class Boss
//------------------------------------------------------------------------------

Boss::Boss() : Monster(SHAPE_BOSS, 0, 0, BOSS_DEFAULT_MAX_HP, BOSS_DEFAULT_MAX_HP, 0, 0, DIR_NONE, BOSS_DEFAULT_ATK, BOSS_DEFAULT_DEF, BOSS_DEFAULT_GOLD, BOSS_DEFAULT_EXP) {
    range = BOSS_DEFAULT_RANGE;
//    brain = new Brain(this);
//    brain = new BrainIQ120(this);
    brain = new BrainIQ150(this);
    rush = false;
    sim->eventQueue->enqueue(new BossRushEvent(sim->time+BOSS_RUSH_INTERVAL, this));
}

void Boss::reset() {
    Monster::reset();
    range = BOSS_DEFAULT_RANGE;
    rush = false;
}

int Boss::getRange() {
    return range;
}

void Boss::setRush(bool rush) {
    this->rush = rush;
}

bool Boss::isBoss() {
    return true;
}

void Boss::move(int dir) {
    if (isDead()) {
        return;
    }
    
    // rush?????? hero??? ?????? ?????? ???????????? ?????? ??????, range??? map??? dim?????? ???????????? ????????? ??????.
    if (rush) {
        if (sim->board->getRowSize() < sim->board->getColSize()) {
            range = sim->board->getColSize();
        }
        else {
            range = sim->board->getRowSize();
        }
    }
    
    // IQ 120/IQ 150????????? ?????? path??? ????????? ????????? ??? ???????????? ???????????? ?????? ????????????.
    if (brain->isPathFound()) {
        Pos *nextPos = brain->getNextPos();

        if (brain->getNextPos() == NULL) {
            exit(1);
        }
        
        if (!sim->board->isBlocked(nextPos->row, nextPos->col) || (sim->board->getUnit(nextPos->row, nextPos->col) == this)) {
            if (sim->board->getUnit(nextPos->row, nextPos->col) != this) {
                sim->board->setUnit(row, col, NULL);
                sim->board->setUnit(nextPos->row, nextPos->col, this); // ??? ???????????? boss??? row, col??? ???????????? ????????????.
            }
            brain->setNextPos();
            if (!brain->isPathFound()) {
                brain->resetPath();
            }
//
//            cout << "holla nextPos->row:" << nextPos->row << " nextPos->col:" << nextPos->col << endl;
        }

        else {
//            cout << nextPos->row << nextPos->col << endl;
            brain->reset();
//            exit(1);
        }
        // ?????? ???????????? ?????? ?????? ?????? ????????????. monster?????? ?????? path??? ?????? ?????? ??? ??????.
        return;
    }
    
    Pos heroPos;
    bool heroFound = brain->findHero(heroPos, range);
    
    if (heroFound) {
        brain->findPath(heroPos);
    }
    else {
        Monster::move(dir);
    }
    // ????????? hero??? ?????????.
    // ?????? hero??? ???????????? monster??? ?????? move()??? ???????????? ??????, brain->isPathFound()??? true??? ??????, ??? ?????? path??? ???????????? ??????.
    
    //--------------------------------------------------------------------------
    // hero??? adjacent?????? ????????? ?????? ?????????.
    // ?????? brain->findHero(heroPos, 1); ????????? ????????? ?????????, ??????????????? ????????????, ????????? ????????? ?????? unit???????????? ??????, ????????? ?????? ???????????? ??????.
    //--------------------------------------------------------------------------
    
    Hero *heroAdjacent = NULL;
    
    if ((heroAdjacent = getHero(row-1, col  )) != NULL ||
        (heroAdjacent = getHero(row,   col+1)) != NULL ||
        (heroAdjacent = getHero(row+1, col  )) != NULL ||
        (heroAdjacent = getHero(row  , col-1)) != NULL) {
        heroAdjacent->decHp(atk);
    }
}

Hero *Boss::getHero(int row, int col) {
    if (!sim->board->isOutOfBoundary(row, col) &&
        sim->board->getUnit(row, col) != NULL &&
        sim->board->getUnit(row, col)->isHero()) {
        return (Hero *)sim->board->getUnit(row, col);
    }
    else {
        return NULL;
    }
}

// battle
void Boss::interact(Unit *unit) {
    Monster::interact(unit);
}
