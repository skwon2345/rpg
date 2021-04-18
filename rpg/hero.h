#ifndef hero_h
#define hero_h

class Unit;
class Inventory;
class Paperdoll;
class Magicbook;
class Backpack;

class Hero : public Unit {
protected:
    Inventory *slot;
    Paperdoll *paperdoll;
    Magicbook *magicSlot;
//    bool withMerchant;
    Backpack *backpack;
    
public:
    Hero();
    virtual void reset();

    virtual ~Hero();
    
    virtual void printStat();
    
    bool isHero();
    
    Inventory *getSlot();
    void setSlot(Inventory *slot);
    
    void openBackpack();
    
    Magicbook *getMagicSlot();
    
//    bool getWithMerchant();
//    void setWithMerchant(bool withMerchant);
    
    virtual void useItem(int index);
    virtual bool equip(Item *itemToEquip);
    virtual void unequip(int bodyPartIDToUnequip);

    virtual void effectMagic(int index);
    
    virtual void move(int dir); // MUST override(implement) the pure virtual function of parent class.
    virtual void interact(Unit *unit);
    virtual void effect(Unit *unit, Item *item, Prop *prop, int skillNumber, Unit *user);
    
//    virtual void buyItem(int index);
    
    virtual bool canReceiveItem();
    // pre-condition: canReceiveItem() == true
    virtual void receiveItem(Item *item);
    virtual void sellItem(Inventory *itemsToBuy, int itemIndexToSell);
    virtual void cancelToSellLastItem(Inventory *itemsToBuy);
};


#endif
