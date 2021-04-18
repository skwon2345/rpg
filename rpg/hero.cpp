#include <iostream>

using namespace std;

#include "main.h"
#include "item.h"
#include "prop.h"
#include "unit.h"
#include "hero.h"
#include "pet.h"
#include "ridable.h"
#include "board.h"
#include "inventory.h"
#include "paperdoll.h"
#include "magic.h"
#include "magicbook.h"
#include "brain.h"
#include "backpack.h"
#include "priority_queue.h"
#include "statistics.h"
#include "event.h"

//extern Board *board;
#include "sim.h"
extern Sim *sim;

//------------------------------------------------------------------------------
// class Hero
//------------------------------------------------------------------------------

Hero::Hero() : Unit(SHAPE_HERO, HERO_START_ROW, HERO_START_COL, HERO_DEFAULT_MAX_HP, HERO_DEFAULT_MAX_HP, HERO_DEFAULT_MAX_MP, HERO_DEFAULT_MAX_MP, DIR_NONE, HERO_DEFAULT_ATK, HERO_DEFAULT_DEF, HERO_DEFAULT_GOLD, HERO_DEFAULT_EXP) {
    slot = new Inventory();
    paperdoll = new Paperdoll();
    magicSlot = new Magicbook();
//    brain = new Brain(this);
    backpack = new Backpack();
}

void Hero::reset() {
    Unit::reset();
    slot->reset();
    paperdoll->reset();
    magicSlot->reset();
    //    brain = new Brain(this);
    backpack->reset();
}

Hero::~Hero() {
    if (slot != NULL) {
        delete slot;
        slot = NULL;
    }
    if (paperdoll != NULL) {
        delete paperdoll;
        paperdoll = NULL;
    }
    if (magicSlot != NULL) {
        delete magicSlot;
        magicSlot = NULL;
    }
}

void Hero::printStat() {
    Unit::printStat();
    cout << endl;

    cout << "             ";
    slot->printIndices(1);
    cout << "        ";
    cout << "             ";
    magicSlot->printIndices(6);
    cout << endl;
    
    cout << "Slot       : ";
    slot->print();
    cout << "        ";
    cout << "Magic Slot : ";
    magicSlot->print();
    cout << endl;
    
    cout << "Backpack   : ";
    backpack->print();
    cout << endl;
    
    cout << "Paperdoll  : ";
    paperdoll->print();
    cout << endl;
}

void Hero::useItem(int index) {
    slot->useItemAt(index, this);
}

bool Hero::equip(Item *itemToEquip) {
    return paperdoll->equip(this, itemToEquip);
}

void Hero::unequip(int bodyPartIDToUnequip) {
    if (!slot->isFull()) {
        Item *itemUnequipped = paperdoll->unequip(this, bodyPartIDToUnequip);
        if (itemUnequipped != NULL) {
            slot->add(itemUnequipped);
        }
    }
}

void Hero::effectMagic(int index) {
    magicSlot->castMagicAt(index, this);
}

void Hero::move(int dir) {
    this->dir = dir;

    if (ride != NULL) {
        ride->move(DIR_NONE);
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
        (sim->board->getProp(nextRow, nextCol) == NULL || sim->board->getProp(nextRow, nextCol)->isClimbable())) {
        sim->board->setUnit(prevRow, prevCol, NULL);
        sim->board->setUnit(nextRow, nextCol, this);
        moved = true;
        sim->statistics->totalNumStepsHero++;
    }
    
    if (sim->board->getProp(nextRow, nextCol) != NULL) {
        sim->board->getProp(nextRow, nextCol)->trigger(this);
    }
    
    if (moved) {
        for (int i = prevRow-1; i <= prevRow+1; i++) {
            for (int j = prevCol-1; j <= prevCol+1; j++) {
                if (!(i == row && j == col) &&
                    !sim->board->isOutOfBoundary(i, j) &&
                    sim->board->getUnit(i, j) != NULL &&
                    sim->board->getUnit(i, j)->isMonster()) {
                    sim->board->getUnit(i, j)->unfreeze();
                }
            }
        }
        
        for (int i = nextRow-1; i <= nextRow+1; i++) {
            for (int j = nextCol-1; j <= nextCol+1; j++) {
                if (!(i == nextRow && j == nextCol) &&
                    !sim->board->isOutOfBoundary(i, j) &&
                    sim->board->getUnit(i, j) != NULL &&
                    sim->board->getUnit(i, j)->isMonster()) {
                    sim->board->getUnit(i, j)->freeze();
                }
            }
        }
    }
    
    if (moved) {
        if (sim->board->getItem(nextRow, nextCol) != NULL &&
            sim->board->getItem(nextRow, nextCol)->isPickupable()) {
            if (!slot->isFull()) {
                slot->add(sim->board->getItem(nextRow, nextCol));
                if (sim->board->getItem(nextRow, nextCol)->isPotion()) {
                    int potionRespawnRandomInterval = (rand() % (POTION_RESPAWN_INTERVAL_MAX - POTION_RESPAWN_INTERVAL_MIN + 1)) + POTION_RESPAWN_INTERVAL_MIN;
                    sim->eventQueue->enqueue(new PotionRespawnEvent(sim->time+potionRespawnRandomInterval, new Potion()));
                }
                sim->board->setItem(nextRow, nextCol, NULL);
            }
        }
    }
    
    if (!moved) {
        if (sim->board->getUnit(nextRow, nextCol) != NULL) {
            sim->board->getUnit(nextRow, nextCol)->interact(this);
        }
    }
}

bool Hero::isHero() {
    return true;
}

Inventory *Hero::getSlot() {
    return slot;
}

void Hero::setSlot(Inventory *slot) {
    this->slot = slot;
}

void Hero::openBackpack() {
    while (true) {
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "------------------------ Suk-Kwon's Backpack ----------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        
        char command = '\0';
        
        cout << "(F)rom backpack, (T)o backpack, or e(X)it: ";
        cin >> command;
        
        if (command == 'f' || command == 'F') {
            backpack->moveFromBackpackTo(slot);
        }
        else if (command == 't' || command == 'T') {
            backpack->moveToBackpackFrom(slot);
        }
        else if (command == 'x' || command == 'X') {
            break;
        }
        else {
            cout << "error: No such command!!" << endl;
        }
    }
}

Magicbook *Hero::getMagicSlot() {
    return magicSlot;
}

//bool Hero::getWithMerchant() {
//    return withMerchant;
//}
//
//void Hero::setWithMerchant(bool withMerchant) {
//    this->withMerchant = withMerchant;
//}

void Hero::interact(Unit *unit) {
}

//#define MAGIC_FIREBALL_NAME "Fireball"
//#define MAGIC_FIREBALL_RANGE 5
//#define MAGIC_FIREBALL_DELAY 1
//#define MAGIC_FIREBALL_SHAPE '@'
//#define MAGIC_FIREBALL_SKILL_NUMBER 0
//#define MAGIC_FIREBALL_ATK 10

// 일단 magicbook에서만 casting한다고 가정할 때..
void Hero::effect(Unit *unit, Item *item, Prop *prop, int skillNumber, Unit *user) {
    magicSlot->effectBySkillNumber(unit, item, prop, skillNumber, user);
}

//void Hero::buyItem(int index) {
//    if (!getMerchant()->sellItemAt(index, this)) {
//        setMerchant(NULL);
//    }
//}

bool Hero::canReceiveItem() {
    return true;
//    return !slot->isFull() || !backpack->isFull();
}

// pre-condition: canReceiveItem() == true
void Hero::receiveItem(Item *item) {
    if (!slot->isFull()) {
        slot->add(item);
    }
    else {
        backpack->insert(item);
    }
}

void Hero::sellItem(Inventory *itemsToBuy, int itemIndexToSell) {
    itemsToBuy->add(slot->removeItemAt(itemIndexToSell));
}

void Hero::cancelToSellLastItem(Inventory *itemsToBuy) {
    decGold(itemsToBuy->getItemAt(itemsToBuy->size() - 1)->getPrice());
    slot->add(itemsToBuy->removeItemAt(itemsToBuy->size() - 1));
}
