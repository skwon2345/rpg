#ifndef PAPERDOLL_H
#define PAPERDOLL_H

class Item;
class Armor;
class Weapon;
class Unit;

class Paperdoll {
private:
    Armor *head;
    Armor *chest;
    Armor *gloves;
    Armor *back;
    Armor *legs;
    Armor *feet;
    Weapon *rightHand;
    Weapon *leftHand;
    
public:
    Paperdoll();
    virtual void reset();
    virtual ~Paperdoll();
    virtual void destroy();

    void print();
    
    bool equip(Unit *unit, Item *itemToEquip);
    Item *unequip(Unit *unit, int bodyPartID);
};

#endif
