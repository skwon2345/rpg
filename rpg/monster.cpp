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
    
    // rush때는 hero가 어디 있든 찾아오게 하기 위해, range를 map의 dim중에 긴쪽으로 정하면 된다.
    if (rush) {
        if (sim->board->getRowSize() < sim->board->getColSize()) {
            range = sim->board->getColSize();
        }
        else {
            range = sim->board->getRowSize();
        }
    }
    
    // IQ 120/IQ 150등에서 찾은 path가 있다면 그것을 다 따라가기 전까지는 계속 따라간다.
    if (brain->isPathFound()) {
        Pos *nextPos = brain->getNextPos();

        if (brain->getNextPos() == NULL) {
            exit(1);
        }
        
        if (!sim->board->isBlocked(nextPos->row, nextPos->col) || (sim->board->getUnit(nextPos->row, nextPos->col) == this)) {
            if (sim->board->getUnit(nextPos->row, nextPos->col) != this) {
                sim->board->setUnit(row, col, NULL);
                sim->board->setUnit(nextPos->row, nextPos->col, this); // 이 코드에서 boss의 row, col도 바뀌므로 주의하자.
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
        // 만약 막혔다면 한번 또는 계속 기다린다. monster등에 의해 path가 잠시 막힐 수 있다.
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
    // 주변에 hero가 없으면.
    // 만약 hero를 찾았으면 monster의 기본 move()를 사용하지 않고, brain->isPathFound()가 true인 동안, 그 찾은 path를 따라가게 된다.
    
    //--------------------------------------------------------------------------
    // hero가 adjacent되어 있다면 한대 때리기.
    // 사실 brain->findHero(heroPos, 1); 이렇게 찾아도 되지만, 대각선까지 찾으므로, 나중에 아래를 따로 unit화하기로 하고, 현재는 그냥 아래처럼 하자.
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
