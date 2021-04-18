#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

#include "main.h"
#include "item.h"
#include "unit.h"
#include "board.h"
#include "shockwave.h"
#include "event.h"
#include "priority_queue.h"

//extern Board *board;
//extern int numTicks;
#include "sim.h"
extern Sim *sim;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// 1st Generation
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// class Item
//------------------------------------------------------------------------------

Item::Item() {
    init('I', string(""), 0, false, false);
}

Item::Item(Item &other) {
    shape = other.shape;
    
    row = other.row;
    col = other.col;
    dir = other.dir;

    name = other.name;
    price = other.price;
    disposable = other.disposable;
    removable = other.removable;
    removed = other.removed;
    pickupable = other.pickupable;
}

Item::Item(char shape, string name, int price, bool disposable, bool removable) {
    init(shape, name, price, disposable, removable);
}

void Item::init(char shape, string name, int price, bool disposable, bool removable) {
    this->shape = shape;
    
    row = -1;
    col = -1;
    dir = DIR_NONE;
    
    this->name = name;
    this->price = price;
    this->disposable = disposable;
    this->removable = removable;
    removed = false;
    pickupable = true;
}

Item::~Item() {
}

void Item::print() {
    cout << shape;
}

void Item::printFullSpec() {
    cout << shape << ":" << setw(30) << left << name << right << setw(7) << price << "g ";
}

char Item::getShape() {
    return shape;
}

int Item::getRow() {
    return row;
}

void Item::setRow(int row) {
    this->row = row;
}

int Item::getCol() {
    return col;
}

void Item::setCol(int col) {
    this->col = col;
}

int Item::getDir() {
    return dir;
}

void Item::setDir(int dir) {
    this->dir = dir;
}

string Item::getName() {
    return name;
}

int Item::getPrice() {
    return price;
}

bool Item::isDisposable() {
    return disposable;
}

bool Item::isRemovable() {
    return removable;
}

bool Item::isPickupable() {
    return pickupable;
}

bool Item::isRemoved() {
    return removed;
}

bool Item::isConsumable() {
    return false;
}

bool Item::isEquippable() {
    return false;
}

bool Item::isPotion() {
    return false;
}

void Item::effect(Unit *unit, Item *item, Prop *prop, Unit *user) {
}

#define NUM_ITEM_TYPES 4
Item *createRandomItem() {
//    int randItemType = rand() % NUM_ITEM_TYPES;
    int randPerc = rand() % 100;
    
    if (randPerc >= 0 && randPerc < 10) { // 10%
        return new Potion();
    }
    else if (randPerc >= 10 && randPerc < 20) { // 10%
        return new Dagger();
    }
    else if (randPerc >= 20 && randPerc < 30) { // 10%
        return new TimeBomb();
    }
    else if (randPerc >= 30 && randPerc < 70) { // 40%
        return createRandomArmor();
    }
    else { // if (randItemType == 3) { // 30%
        return createRandomWeapon();
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// 2nd Generation
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// class TimeBomb
//------------------------------------------------------------------------------
TimeBomb::TimeBomb() : Item(SHAPE_TIMEBOMB, string(TIMEBOMB_NAME), TIMEBOMB_PRICE, TIMEBOMB_DISPOSABLE, TIMEBOMB_REMOVABLE) {
    atk = TIMEBOMB_DEFAULT_ATK;
    range = TIMEBOMB_DEFAULT_RANGE;
}

TimeBomb::TimeBomb(int atk,int range) : Item(SHAPE_TIMEBOMB, string(TIMEBOMB_NAME), TIMEBOMB_PRICE, TIMEBOMB_DISPOSABLE, TIMEBOMB_REMOVABLE) {
    this->atk = atk;
    this->range = range;
}

TimeBomb::TimeBomb(TimeBomb &other) : Item(other) {
    this->atk = other.atk;
    this->range = other.range;
}

void TimeBomb::explode() {
    sim->board->startWave(NULL, this, NULL,
                          row, col,
                          range,
                          5,
                          '@',
                          -1);
    sim->board->setItem(row, col, NULL);
    row = -1;
    col = -1;
//    delete this; // 여기에서 지우면, shockwave에서 callback하려 할 때 에러남. 이미 지워진 object를 access하게 됨.
}

void TimeBomb::effect(Unit *unit, Item *item, Prop *prop, Unit *user) {
    if (unit != NULL) {
        unit->decHp(atk);
        
        if (unit->isMonster() && unit->isDead()) {
            user->incGold(unit->getGold());
            user->incExp(unit->getExp());
            unit->decGold(unit->getGold());
            unit->decExp(unit->getExp());
        }
    }
}

bool TimeBomb::use(Unit *unit) {
    if (unit->getDir() == DIR_N) {
        sim->board->setItem(unit->getRow() + 1, unit->getCol(), this);
    }
    else if (unit->getDir() == DIR_E) {
        sim->board->setItem(unit->getRow(), unit->getCol() - 1, this);
    }
    else if (unit->getDir() == DIR_S) {
        sim->board->setItem(unit->getRow() - 1, unit->getCol(), this);
    }
    else if (unit->getDir() == DIR_W) {
        sim->board->setItem(unit->getRow(), unit->getCol() + 1, this);
    }
    else {
        cout << "TimeBomb::use() error : Unit's direction is DIR_NONE. " << endl;
        return false;
    }
    
    sim->eventQueue->enqueue(new TimeBombExplodeEvent(sim->time+TIMEBOMB_EXPLODE_INTERVAL, this));
    removed = true;
    pickupable = false;
    return true;
}

Item *TimeBomb::clone() {
    return new TimeBomb(*this); // calling copy contructor to copy all the data from me

}

//------------------------------------------------------------------------------
// class ConsumableItem
//------------------------------------------------------------------------------

ConsumableItem::ConsumableItem() : Item() {
}

ConsumableItem::ConsumableItem(ConsumableItem &other) : Item(other) {
}

ConsumableItem::ConsumableItem(char shape, string name, int price, bool disposable, bool removable)
    : Item(shape, name, price, disposable, removable) {
}

ConsumableItem::~ConsumableItem() {
}

void ConsumableItem::printFullSpec() {
    Item::printFullSpec();
}

bool ConsumableItem::isConsumable() {
    return true;
}


//------------------------------------------------------------------------------
// class EquippableItem
//------------------------------------------------------------------------------

EquippableItem::EquippableItem() : Item() {
    durability = 0;
}

EquippableItem::EquippableItem(EquippableItem &other) : Item(other) {
    durability = other.durability;
}

EquippableItem::EquippableItem(char shape, string name, int price, bool disposable, bool removable, int durability)
    : Item(shape, name, price, disposable, removable) {
    this->durability = durability;
}

EquippableItem::~EquippableItem() {
}

void EquippableItem::printFullSpec() {
    Item::printFullSpec();
}

bool EquippableItem::isEquippable() {
    return true;
}

bool EquippableItem::isArmor() {
    return false;
}

bool EquippableItem::isWeapon() {
    return false;
}

//------------------------------------------------------------------------------
// class ThrowableItem
//------------------------------------------------------------------------------

ThrowableItem::ThrowableItem() : EquippableItem() {
}

ThrowableItem::ThrowableItem(ThrowableItem &other) : EquippableItem(other) {
}

ThrowableItem::ThrowableItem(char shape, string name, int price, bool disposable, bool removable, int durability)
    : EquippableItem(shape, name, price, disposable, removable, durability) {
}

ThrowableItem::~ThrowableItem() {
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// 3rd Generation
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// class Potion
//------------------------------------------------------------------------------

Potion::Potion() : ConsumableItem(SHAPE_POTION, string(POTION_NAME), POTION_PRICE, POTION_DISPOSABLE, POTION_REMOVABLE) {
    hp = POTION_DEFAULT_HP;
}

Potion::Potion(Potion &other) : ConsumableItem(other) {
    hp = other.hp;
}

Potion::Potion(int hp) : ConsumableItem(SHAPE_POTION, string(POTION_NAME), POTION_PRICE, POTION_DISPOSABLE, POTION_REMOVABLE) {
    this->hp = hp;
}

Potion::~Potion() {
}

bool Potion::isPotion() {
    return true;
}

//void Potion::printFullSpec() {
//    Item::ConsumableItem();
//    
//}

bool Potion::use(Unit *unit) {
//    cout << "bool Potion::use(Unit *unit)" << endl;
    return unit->incHp(hp); // stub code
}

Item *Potion::clone() {
    return new Potion(*this); // calling copy contructor to copy all the data from me
}

//------------------------------------------------------------------------------
// class Potion
//------------------------------------------------------------------------------
Dagger::Dagger() : ThrowableItem(SHAPE_DAGGER, string(DAGGER_NAME), DAGGER_PRICE, DAGGER_DISPOSABLE, DAGGER_REMOVABLE, DAGGER_DURABILITY) {
    numTicksWhenMoved = -1;
    range = DAGGER_DEFAULT_RANGE;
    atk = DAGGER_DEFAULT_ATK;
    curFlyingDist = 0;
}

Dagger::Dagger(Dagger &other) : ThrowableItem(other) {
    numTicksWhenMoved = other.numTicksWhenMoved;
    range = other.range;
    atk = other.atk;
    curFlyingDist = 0;
}

Dagger::Dagger(int range, int atk) : ThrowableItem(SHAPE_DAGGER, string(DAGGER_NAME), DAGGER_PRICE, DAGGER_DISPOSABLE, DAGGER_REMOVABLE, DAGGER_DURABILITY) {
    numTicksWhenMoved = -1;
    this->range = range;
    this->atk = atk;
    curFlyingDist = 0;
}

Dagger::~Dagger() {
}

int Dagger::getNumTicksWhenMoved() {
    return numTicksWhenMoved;
}

void Dagger::setNumTicksWhenMoved(int numTicksWhenMoved) {
    this->numTicksWhenMoved = numTicksWhenMoved;
}

int Dagger::getRange() {
    return range;
}

void Dagger::setRange(int range) {
    this->range = range;
}

int Dagger::getAtk() {
    return atk;
}

void Dagger::setAtk(int atk) {
    this->atk = atk;
}

int Dagger::getCurFlyingDist() {
    return curFlyingDist;
}

void Dagger::incCurFlyingDist() {
    curFlyingDist++;
}

void Dagger::resetCurFlyingDist() {
    curFlyingDist = 0;
}

bool Dagger::use(Unit *unit) {
    cout << "bool Dagger::use(Unit *unit)" << endl;
    if (unit != NULL) {
        if (unit->getDir() == DIR_N) {
            cout << "bool Dagger::use(Unit *unit): DIR_N" << endl;

            if (!sim->board->isOutOfBoundary(unit->getRow() - 1, unit->getCol())) {
                dir = unit->getDir();
                numTicksWhenMoved = sim->time;
                sim->board->setThrowing(unit->getRow() - 1, unit->getCol(), this);
                removed = true;
                return true;
            }
        }
        else if (unit->getDir() == DIR_E) {
            cout << "bool Dagger::use(Unit *unit): DIR_E" << endl;
            
            if (!sim->board->isOutOfBoundary(unit->getRow(), unit->getCol() + 1)) {
                dir = unit->getDir();
                numTicksWhenMoved = sim->time;
                sim->board->setThrowing(unit->getRow(), unit->getCol() + 1, this);
                removed = true;
                return true;
            }
        }
        else if (unit->getDir() == DIR_S) {
            cout << "bool Dagger::use(Unit *unit): DIR_S" << endl;
            
            if (!sim->board->isOutOfBoundary(unit->getRow() + 1, unit->getCol())) {
                dir = unit->getDir();
                numTicksWhenMoved = sim->time;
                sim->board->setThrowing(unit->getRow() + 1, unit->getCol(), this);
                removed = true;
                return true;
            }
        }
        else if(unit->getDir() == DIR_W) {
            cout << "bool Dagger::use(Unit *unit): DIR_W" << endl;

            if (!sim->board->isOutOfBoundary(unit->getRow(), unit->getCol() - 1)) {
                dir = unit->getDir();
                numTicksWhenMoved = sim->time;
                sim->board->setThrowing(unit->getRow(), unit->getCol() - 1, this);
                removed = true;
                return true;
            }
        }
    }
    return false;
}

Item *Dagger::clone() {
    return new Dagger(*this); // calling copy contructor to copy all the data from me
}

//------------------------------------------------------------------------------
// class Armor
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Armor related
//------------------------------------------------------------------------------
#define ITEM_ID_ARMOR "Armor"

#define DEFAULT_ARMOR_SHAPE 'A'
#define DEFAULT_ARMOR_NAME "Armor"
#define DEFAULT_ARMOR_PRICE 0
#define DEFAULT_ARMOR_DURABILITY 10
#define DEFAULT_ARMOR_DEF 10

//#define ARMOR_BODYPART_ID_NONE -1
//#define ARMOR_BODYPART_ID_HEAD 0
//#define ARMOR_BODYPART_ID_CHEST 1
//#define ARMOR_BODYPART_ID_GLOVES 2
//#define ARMOR_BODYPART_ID_BACK 3
//#define ARMOR_BODYPART_ID_PANTS 4
//#define ARMOR_BODYPART_ID_BOOTS 5
//#define WEAPON_BODYPART_ID_RIGHT_HAND 6
//#define WEAPON_BODYPART_ID_LEFT_HAND 7
//
//#define ARMOR_HEAD_SHAPE 'A'
//#define ARMOR_CHEST_SHAPE 'W'
//#define ARMOR_GLOVES_SHAPE 'U'
//#define ARMOR_BACK_SHAPE 'Q'
//#define ARMOR_PANTS_SHAPE 'E'
//#define ARMOR_BOOTS_SHAPE 'Z'
//#define WEAPON_ONE_HAND_SHAPE '!'
//#define WEAPON_TWO_HAND_SHAPE '?'
//
//#define WEAPON_ONE_HAND 1
//#define WEAPON_TWO_HAND 2
//    int def;
//    int bodyPartID;
Armor::Armor() : EquippableItem() {
    def = 0;
    bodyPartID = ARMOR_BODYPART_ID_NONE;
}

Armor::Armor(Armor &other) : EquippableItem(other) {
    def = other.def;
    bodyPartID = other.bodyPartID;
}

Armor::Armor(char shape, string name, int price, bool disposable, bool removable, int durability, int def, int bodyPartID)
    : EquippableItem(shape, name, price, disposable, removable, durability) {
    this->def = def;
    this->bodyPartID = bodyPartID;
}

Armor::~Armor() {
}

void Armor::printFullSpec() {
    EquippableItem::printFullSpec();
    
    cout << "def(" << setw(6) << def << ")";
}

int Armor::getDef() {
    return def;
}

int Armor::getBodyPartID() {
    return bodyPartID;
}

bool Armor::isArmor() {
    return true;
}

bool Armor::use(Unit *unit) {
    if (unit == NULL) {
        cout << "Weapon::use(): if (unit == NULL) {" << endl;
        exit(1);
    }
    
    if (unit->equip(this)) {
        removed = true;
        return true;
    }
    else {
        return false;
    }
}

Armor *createRandomArmor() {
    int randBodyPartID = rand() % (WEAPON_BODYPART_ID_LEFT_HAND + 1);
//    cout << "Armor::createRandomArmor(): pBodyPartID: " << randBodyPartID << endl;
    return getRandomArmor(randBodyPartID);
}

Armor *getRandomArmorByName(string name) {
    while (true) {
        Armor *curArmor = createRandomArmor();
        if (curArmor->getName() == name) {
            return curArmor;
        }
        else {
            delete curArmor;
        }
    }
    return NULL;
}

#define MAX_NUM_RANDOM_ARMORS 18
// post-condition: id there is no such body part then random item will be return.
Armor *getRandomArmor(int pBodyPartID) {
    char shape[MAX_NUM_RANDOM_ARMORS] =     { ARMOR_HEAD_SHAPE,            ARMOR_CHEST_SHAPE,              ARMOR_GLOVES_SHAPE,       ARMOR_BACK_SHAPE,       ARMOR_LEGS_SHAPE,       ARMOR_FEET_SHAPE,         ARMOR_HEAD_SHAPE,            ARMOR_CHEST_SHAPE,         ARMOR_GLOVES_SHAPE,         ARMOR_BACK_SHAPE,      ARMOR_LEGS_SHAPE,       ARMOR_FEET_SHAPE,       ARMOR_HEAD_SHAPE,         ARMOR_CHEST_SHAPE,            ARMOR_BACK_SHAPE,            ARMOR_GLOVES_SHAPE,       ARMOR_LEGS_SHAPE,        ARMOR_FEET_SHAPE,        };
    char name[MAX_NUM_RANDOM_ARMORS][80] =  {"Headgear for Brace Faces",   "Sassy debater\'s suit jacket", "Gudeunsaal",             "back braces",          "lululemon yoga pants",  "Ganzi riding boots",      "the rock head of a dummy",  "Authentic Six-pack",      "NoNamedGLoves",            "NameThisBaCKArmour",  "3050SkinnyJeans",       "Hermes\' Flyingboots",  "CinderellaEffectHelmet",  "CinderellaEffectChest",      "CinderellaEffectBackplate", "CinderellaEffectGloves", "CinderellaEffectPants",  "CinderellaEffectBoots", };
    int price[MAX_NUM_RANDOM_ARMORS] =      { 100,                         10000,                          1000000,                  1000,                   100,                     1000,                      100000,                      1000000,                   100,                        1000,                  10000,                   1000000,                 10000000,                  10000000,                    10000000,                    10000000,                 10000000,                 10000000,                 };
    int durability[MAX_NUM_RANDOM_ARMORS] = { 100,                         1000,                           1000000,                  1000,                   100,                     100,                       10000,                       100000,                    100,                        1000,                  10000,                   1000000,                 10000000,                  10000000,                    10000000,                    10000000,                 10000000,                 10000000,                 };
    int def[MAX_NUM_RANDOM_ARMORS] =        { 100,                         1000,                           10000,                    1000,                   100,                     1000,                      10000,                       100000,                    100,                        1000,                  10000,                   1000000,                 10000000,                  10000000,                    10000000,                    10000000,                 10000000,                 10000000,                 };
    int bodyPartID[MAX_NUM_RANDOM_ARMORS] = { ARMOR_BODYPART_ID_HEAD,      ARMOR_BODYPART_ID_CHEST,        ARMOR_BODYPART_ID_GLOVES, ARMOR_BODYPART_ID_BACK, ARMOR_BODYPART_ID_LEGS, ARMOR_BODYPART_ID_FEET,   ARMOR_BODYPART_ID_HEAD,      ARMOR_BODYPART_ID_CHEST,   ARMOR_BODYPART_ID_GLOVES,   ARMOR_BODYPART_ID_BACK, ARMOR_BODYPART_ID_LEGS, ARMOR_BODYPART_ID_FEET, ARMOR_BODYPART_ID_HEAD,    ARMOR_BODYPART_ID_CHEST,      ARMOR_BODYPART_ID_BACK,      ARMOR_BODYPART_ID_GLOVES, ARMOR_BODYPART_ID_LEGS,  ARMOR_BODYPART_ID_LEGS,  };
    
    int randItemIndex = rand() % MAX_NUM_RANDOM_ARMORS;
    
    if (pBodyPartID != -1) {
        for (int i = 0; i < RANDOM_EQUIPPABLE_ITEM_MAX_ITER && pBodyPartID != bodyPartID[randItemIndex]; i++) {
            randItemIndex = rand() % MAX_NUM_RANDOM_ARMORS;
        }
    }
    
    return new Armor(shape[randItemIndex],
                     name[randItemIndex],
                     price[randItemIndex],
                     false,
                     true,
                     durability[randItemIndex],
                     def[randItemIndex],
                     bodyPartID[randItemIndex]);
}

Item *Armor::clone() {
    return new Armor(*this); // calling copy contructor to copy all the data from me
}

//------------------------------------------------------------------------------
// class Weapon
//------------------------------------------------------------------------------

//class Weapon : public EquippableItem {
//protected:
//    int atk;
//    int numHands;
Weapon::Weapon() : EquippableItem() {
    atk = 0;
    numHands = 0;
}

Weapon::Weapon(Weapon &other) : EquippableItem(other) {
    atk = other.atk;
    numHands = other.numHands;
}

Weapon::Weapon(char shape, string name, int price, bool disposable, bool removable, int durability, int atk, int numHands)
    : EquippableItem(shape, name, price, disposable, removable, durability) {
    this->atk = atk;
    this->numHands = numHands;
}

Weapon::~Weapon() {
}

void Weapon::printFullSpec() {
    EquippableItem::printFullSpec();
    
    cout << "atk(" << setw(6) << atk << ")";
}

int Weapon::getAtk() {
    return atk;
}

int Weapon::getNumHands() {
    return numHands;
}

bool Weapon::isWeapon() {
    return true;
}

bool Weapon::use(Unit *unit) {
    if (unit == NULL) {
        cout << "Weapon::use(): if (unit == NULL) {" << endl;
        exit(1);
    }
    
    if (unit->equip(this)) {
        removed = true;
        return true;
    }
    else {
        return false;
    }
}

Item *Weapon::clone() {
    return new Weapon(*this); // calling copy contructor to copy all the data from me
}

Weapon *createRandomWeapon() {
    int randNumHands = rand() % 2;
    
    return getRandomWeapon(randNumHands);
}

Weapon *getRandomWeaponByName(string name) {
    while (true) {
        Weapon *curWeapon = createRandomWeapon();
        if (curWeapon->getName() == name) {
            return curWeapon;
        }
        else {
            delete curWeapon;
        }
    }
    return NULL;
}

/*
 #define WEAPON_ONE_HAND_SHAPE '!'
 #define WEAPON_TWO_HAND_SHAPE '?'
 */
#define MAX_NUM_RANDOM_WEAPONS 8
Weapon *getRandomWeapon(int pNumHands) {
    char shape[MAX_NUM_RANDOM_WEAPONS] =     {WEAPON_ONE_HAND_SHAPE, WEAPON_TWO_HAND_SHAPE,  WEAPON_ONE_HAND_SHAPE,           WEAPON_TWO_HAND_SHAPE,             WEAPON_ONE_HAND_SHAPE,        WEAPON_TWO_HAND_SHAPE,   WEAPON_ONE_HAND_SHAPE, WEAPON_TWO_HAND_SHAPE};
    char name[MAX_NUM_RANDOM_WEAPONS][80] =  {"The leg of Craken",   "Very Heavy Riceballs", "Ultra monster repellent spray",  "Moldy Cheese (gloves provided)",  "Super thick math workbook",  "Lovely sharp needles",  "Toxic Waste",        "CinderellaEffectSword"};
    int price[MAX_NUM_RANDOM_WEAPONS] =      {1000,                    10000,                  5000,                            10,                                50000,                       52,                      88,                    10000000};
    int durability[MAX_NUM_RANDOM_WEAPONS] = {510,                     100,                    500,                             10,                                1001,                        24,                      8,                     10000000};
    int atk[MAX_NUM_RANDOM_WEAPONS] =        {20,                      100,                    50,                              10,                                88,                          8,                       7,                     10000000};
    int numHands[MAX_NUM_RANDOM_WEAPONS] =   {1,                       2,                      1,                               2,                                 1,                           2,                       1,                     2};
    
    int randItemIndex = rand() % MAX_NUM_RANDOM_WEAPONS;
    if (pNumHands != -1) {
        for (int i = 0; i < RANDOM_EQUIPPABLE_ITEM_MAX_ITER && pNumHands != numHands[randItemIndex]; i++) {
            randItemIndex = rand() % MAX_NUM_RANDOM_WEAPONS;
        }
    }
    
    return new Weapon(shape[randItemIndex],
                      name[randItemIndex],
                      price[randItemIndex],
                      false,
                      true,
                      durability[randItemIndex],
                      atk[randItemIndex],
                      numHands[randItemIndex]);
}
