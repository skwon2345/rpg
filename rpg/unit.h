#ifndef UNIT_H
#define UNIT_H

#include "main.h"

class Item;
class Prop;
class Hero;
class Board;
class Inventory;
class Ridable;
class Merchant;
class Brain;
class Pet;
class HellDog;

class Unit { // abstract class
protected:
    char shape;
    
    int row;
    int col;
    
    int dir;
    
    int hp;
    int maxHp;
    int mp;
    int maxMp;
    
    int atk;
    int def;

    int gold;
    int exp;

//    Item *pocket;
    
//    Board *board;
    
    bool died;
    bool frozen;
    
    Ridable *ride;
    
    Brain *brain;
    
    Pet *pet;
//    Merchant *merchant;
    
public:
    Unit();
    Unit(Unit &other);
    Unit(char shape, int row, int col, int hp, int maxHp, int mp, int maxMp, int dir, int atk, int def, int gold, int exp);
    void init(char shape, int row, int col, int hp, int maxHp, int mp, int maxMp, int dir, int atk, int def, int gold, int exp);
    virtual void reset();
    
    virtual ~Unit();
    
    void print();
    virtual void printStat();
    
//    void setBoard(Board *board);
    
    int getRow();
    void setRow(int row);
    
    int getCol();
    void setCol(int col);
    
    int getDir();
    void setDir(int dir);
    
    char getShape();
    
    bool incHp(int hpInc);
    void decHp(int hpDec);

    int getMp();
    bool incMp(int mpInc);
    void decMp(int mpDec);

    int getAtk();
    void incAtk(int atkInc);
    void decAtk(int atkDec);

    int getDef();
    void incDef(int defInc);
    void decDef(int defDec);

    int getGold();
    void incGold(int gold);
    bool decGold(int gold);
    
    int getExp();
    void incExp(int exp);
    void decExp(int exp);
    
    int getIq();
//    Brain *getBrain();
    
    bool isDead();
        
    void freeze();
    void unfreeze();
    bool isFrozen();
    
//    bool getRide();
//    void setRide(bool ride);
    
    virtual bool isHero();
    virtual bool isMonster();
    virtual bool isBoss();
    
    virtual bool isMerchant();
    virtual bool canReceiveItem();
    // pre-condition: canReceiveItem() == true
    virtual void receiveItem(Item *item);
    virtual void sellItem(Inventory *itemsToBuy, int itemIndexToSell);
    virtual void cancelToSellLastItem(Inventory *itemsToBuy);
    
    virtual bool isRidable();
    virtual void setRide(Ridable *ride);
    virtual bool getOff();
    
    
    virtual bool isPet();
    virtual void setPet(Pet *pet);
    
//    Merchant *getMerchant();
//    virtual void setMerchant(Merchant *merchant);
    
    virtual void useItem(int index); // virtual function: ?????? child classes?????? ??? ???????????? ?????? ?????? ?????? ??????.. ?????? ??????, ???????????? ??????, ??? ????????? ?????? ??????.. parent class?????? fake body??? ????????? ????????? ?????? ?????????.
    virtual bool equip(Item *itemToEquip);
    virtual void unequip(int bodyPartIDToUnequip);
    
    virtual void move(int dir) = 0; // pure virtual function: ?????? ?????? child classes??? ??? ???????????? implemnetation??? ????????? ?????????, ?????????(force) ???????????? ?????? ?????? ??????, pure virtual function??? ??????.
    virtual void interact(Unit *unit) = 0;
    virtual void castMagic(string magicName);
    
    virtual void effect(Unit *unit, Item *item, Prop *prop, int skillNumber, Unit *user);
};

#endif
