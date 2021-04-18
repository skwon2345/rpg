#ifndef MERCHANT_H
#define MERCHANT_H

class Unit;
class Inventory;

class Merchant : public Unit {
protected:
    Inventory *itemsToSell;
    Inventory *itemsToBuy;
    
    void buy(Unit *unit);
    void sell(Unit *unit);
public:
    Merchant();
    Merchant(char shape, int row, int col, int hp, int maxHp, int mp, int maxMp, int dir, int atk, int def, int gold, int exp);
    virtual void reset();

    virtual ~Merchant();
    
    void printItems();
    
    bool isMerchant();
    
    //    virtual bool sellItemAt(int index, Unit *unit);
    
    virtual void move(int dir);
    virtual void interact(Unit *unit);
};


#endif
