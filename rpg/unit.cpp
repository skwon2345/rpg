#include <iostream>

using namespace std;

#include "main.h"
#include "item.h"
#include "prop.h"
#include "unit.h"
#include "ridable.h"
#include "hero.h"
#include "board.h"
#include "inventory.h"
#include "brain.h"
#include "sim.h"
#include "statistics.h"

//extern Board *board;
extern Sim *sim;

//------------------------------------------------------------------------------
// class Unit
//------------------------------------------------------------------------------

Unit::Unit() {
    init('U', -1, -1, 0, 0, 0, 0, DIR_NONE, 0, 0, 0, 0);
}

Unit::Unit(Unit &other) {
    init(other.shape, other.row, other.col, other.hp, other.maxHp, other.mp, other.maxMp, other.dir, other.atk, other.def, other.gold, other.exp);
}

Unit::Unit(char shape, int row, int col, int hp, int maxHp, int mp, int maxMp, int dir, int atk, int def, int gold, int exp) {
    init(shape, row, col, hp, maxHp, mp, maxMp, dir, atk, def, gold, exp);
}

void Unit::init(char shape, int row, int col, int hp, int maxHp, int mp, int maxMp, int dir, int atk, int def, int gold, int exp) {
    this->shape = shape;
    
    this->row = row;
    this->col = col;
    
    this->hp = hp;
    this->maxHp = maxHp;
    this->mp = mp;
    this->maxMp = maxMp;

    this->dir = dir;

    this->atk = atk;
    this->def = def;
    
    this->gold = gold;
    this->exp = exp;
    
    died = false;
    frozen = false;
    
    ride = NULL;
    
    pet = NULL;
    
//    brain = new Brain(this);
    brain = NULL;
}

void Unit::reset() {
//    this->shape = shape;
    
    this->row = -1;
    this->col = -1;
    
    this->hp = maxHp;
//    this->maxHp = maxHp;
    this->mp = maxMp;
//    this->maxMp = maxMp;
    
    this->dir = DIR_NONE;
    
//    this->atk = atk;
//    this->def = def;
    
//    this->gold = gold;
//    this->exp = exp;
    
    died = false;
    frozen = false;
    
    ride = NULL;
    
    pet = NULL;
    
//    brain = new Brain(this);
    if (brain != NULL) {
        brain->reset();
    }
}

Unit::~Unit() {
    // ride는 tile에 의해서 지워진다.
    
    if (brain != NULL) {
        delete brain;
        brain = NULL;
    }
    
    // pet은 tile에 의해서 지워진다.
}

void Unit::print() {
    cout << shape;
}

void Unit::printStat() {
    cout << "[" << shape << "]: HP(" << hp << "/" << maxHp << ") MP(" << mp << "/" << maxMp << ") ATK(" << atk << ") DEF(" << def << ")";
    
//    cout << " Pocket: ";
//    if (pocket == NULL) {
//        cout << " ";
//    }
//    else {
//        pocket->print();
//    }
    
    cout << " Gold: " << gold;
    cout << " Exp: " << exp;
    cout << " row: " << row;
    cout << " col: " << col;
}

//void Unit::setBoard(Board *board) {
//    this->board = board;
//}

int Unit::getRow() {
    return row;
}

void Unit::setRow(int row) {
    this->row = row;
}

int Unit::getCol() {
    return col;
}

void Unit::setCol(int col) {
    this->col = col;
}

int Unit::getDir() {
    return dir;
}

void Unit::setDir(int dir) {
    this->dir = dir;
}

char Unit::getShape() {
    return shape;
}

bool Unit::incHp(int hpInc) {
    if (hp == maxHp) {
        return false;
    }
    
    if (hp + hpInc >= maxHp) {
        hp = maxHp;
    }
    else {
        hp += hpInc;
    }
    
    return true;
}

void Unit::decHp(int hpDec) {
    if (hp - hpDec <= 0) {
        hp = 0;
        died = true;
    }
    else {
        hp -= hpDec;
    }
}

int Unit::getMp() {
    return mp;
}

bool Unit::incMp(int mpInc) {
    if (mp == maxMp) {
        return false;
    }
    
    if (mp + mpInc >= maxMp) {
        mp = maxMp;
    }
    else {
        mp += mpInc;
    }
    
    return true;
}

void Unit::decMp(int mpDec) {
    if (mp - mpDec <= 0) {
        mp = 0;
    }
    else {
        mp -= mpDec;
    }
}

// 이 rpg에서는 오직 weapon과 armor로만 atk와 def를 더하고 빼므로, 0 아래로 떨어질 일은 없다. 또한 max는 inf이다.

int Unit::getAtk() {
    return atk;
}

void Unit::incAtk(int atkInc) {
    atk += atkInc;
}

void Unit::decAtk(int atkDec) {
    atk -= atkDec;
}

int Unit::getDef() {
    return def;
}

void Unit::incDef(int defInc) {
    def += defInc;
}

void Unit::decDef(int defDec) {
    def -= defDec;
}

int Unit::getGold() {
    return gold;
}

void Unit::incGold(int gold) {
    this->gold += gold;
}

bool Unit::decGold(int gold) {
    if (this->gold < gold) {
        return false;
    }
    this->gold -= gold;
    return true;
}

int Unit::getExp() {
    return exp;
}

void Unit::incExp(int exp) {
    this->exp += exp;
}

void Unit::decExp(int exp) {
    this->exp -= exp;
}

int Unit::getIq() {
    return brain->getIq();
}

//Brain *Unit::getBrain() {
//    return brain;
//}

bool Unit::isDead() {
    return died;
}

bool Unit::isHero() {
    return false;
}

bool Unit::isMonster() {
    return false;
}

bool Unit::isBoss() {
    return false;
}

bool Unit::isMerchant() {
    return false;
}

bool Unit::canReceiveItem() {
    return false;
}

// pre-condition: canReceiveItem() == true
void Unit::receiveItem(Item *item) {
}

void Unit::sellItem(Inventory *itemsToBuy, int itemIndexToSell) {
}

void Unit::cancelToSellLastItem(Inventory *itemsToBuy) {
}

bool Unit::isRidable() {
    return false;
}

void Unit::setRide(Ridable *ride) {
    this->ride = ride;
}

bool Unit::getOff() {
    if (ride != NULL) {
//        ride->kickOut();
        // 반드시 주변에 빈 공간이 하나라도 있다고 가정하자. 만약에 진짜로 없다면, 그냥 안내리게 된다.
        for (int i = row - 1 ; i <= row + 1; i++) {
            for (int j = col - 1; j <= col + 1; j++) {
                if (i != row || j != col) { // if (!(i == row && j == col)) { // hero의 위치를 뺀, 주변 8개의 tile을 access
                    if (!sim->board->isOutOfBoundary(i,j) && !sim->board->isBlocked(i,j) && sim->board->getItem(i,j) == NULL) {
                        //                    cout << this->getRow() << " : " << this->getCol() << endl;
                        //                    cout << "i: " << i << "j: " << j << endl;
                        //                    exit(1);
                        sim->board->setUnit(i, j, this);
//                        rider->getOff();
                        ride->setRider(NULL);
                        ride = NULL;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Unit::isPet() {
    return false;
}

void Unit::setPet(Pet *pet) {
    this->pet = pet;
}


//Merchant *Unit::getMerchant() {
//    return merchant;
//}
//
//void Unit::setMerchant(Merchant *merchant) {
//    this->merchant = merchant;
//}

void Unit::freeze() {
    frozen = true;
}

void Unit::unfreeze() {
    frozen = false;
}

bool Unit::isFrozen() {
    return frozen;
}

//bool Unit::getRide() {
//    return ride;
//}
//
//void Unit::setRide(bool ride) {
//    this->ride = ride;
//}

void Unit::useItem(int index) { // fake body
}

bool Unit::equip(Item *itemToEquip) {
    return false;
}

void Unit::unequip(int bodyPartIDToUnequip) {
}

void Unit::castMagic(string magicName) { // fake body
}

void Unit::effect(Unit *unit, Item *item, Prop *prop, int skillNumber, Unit *user) { // fake body
}
