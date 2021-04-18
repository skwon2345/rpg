#ifndef ITEM_H
#define ITEM_H

class Unit;
class Board;
class Prop;
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// 1st Generation
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

class Item {
protected:
    char shape;
    
    // 아래의 row/col/dir은 dagger처럼 Tile의 throwing layer에서 날라갈 때등에서 쓰인다. 보통의 경우 쓰이지 않음.
    int row;
    int col;
    int dir;
    
    string name;
    int price;
    
    bool disposable; // 나는 성공적으로 use되었을 경우, inventory로부터 없어져도 되고, 심지어는 delete되어 heap으로부터도 즉시 사라져야 한다.
    bool removable;  // 나는 성공적으로 use되었을 경우, inventory로부터 없어져도 되지만, 다른곳으로 옮겨진 케이스가 대부분이므로(armor나 weapon처럼) 절대 delete하지 말 것.
    
    bool removed;    // removable일 경우, 이 removed가 true면 inventory에서 지우면 된다.
    bool pickupable; // bomb의 경우, 한번 use되어 땅에 떨어지면 곧 터지므로 줍지 못하도록..

public:
    Item();
    Item(Item &other);
    Item(char shape, string name, int price, bool disposable, bool removable);
    void init(char shape, string name, int price, bool disposable, bool removable);
    virtual ~Item();

    void print();
    virtual void printFullSpec();

    char getShape();
    
    int getRow();
    void setRow(int row);
    
    int getCol();
    void setCol(int col);
    
    int getDir();
    void setDir(int dir);
    
    string getName();
    
    int getPrice();
    
    bool isDisposable();
    bool isRemovable();
    bool isRemoved();
    bool isPickupable();

    virtual bool isConsumable();
    virtual bool isEquippable();
    virtual bool isPotion();
    
    virtual void effect(Unit *unit, Item *item, Prop *prop, Unit *user);
    
    virtual bool use(Unit *unit) = 0; // pure virtual function
    
    virtual Item *clone() = 0; // pure virtual function
};

Item *createRandomItem();

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// 2nd Generation
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class TimeBomb : public Item {
protected:
    int atk;
    int range;
public:
    TimeBomb();
    TimeBomb(int atk,int range);
    TimeBomb(TimeBomb &other);
    
    void explode();
    
    virtual void effect(Unit *unit, Item *item, Prop *prop, Unit *user);
    
    virtual bool use(Unit *unit);
    
    Item *clone();
};

class ConsumableItem : public Item {
protected:
public:
    ConsumableItem();
    ConsumableItem(ConsumableItem &other);
    ConsumableItem(char shape, string name, int price, bool disposable, bool removable);
    virtual ~ConsumableItem();
    
    virtual void printFullSpec();

    bool isConsumable();
};

class EquippableItem : public Item {
protected:
    int durability;
    
public:
    EquippableItem();
    EquippableItem(EquippableItem &other);
    EquippableItem(char shape, string name, int price, bool disposable, bool removable, int durability);
    virtual ~EquippableItem();
    
    virtual void printFullSpec();

    bool isEquippable();
    
    virtual bool isArmor();
    virtual bool isWeapon();
};

// interface
class ThrowableItem : public EquippableItem {
protected:
public:
    ThrowableItem();
    ThrowableItem(ThrowableItem &other);
    ThrowableItem(char shape, string name, int price, bool disposable, bool removable, int durability);
    virtual ~ThrowableItem();
    
    virtual int getNumTicksWhenMoved() = 0;
    virtual void setNumTicksWhenMoved(int numTicksWhenMoved) = 0;
    
    virtual int getRange() = 0;
    virtual void setRange(int range) = 0;
    
    virtual int getAtk() = 0;
    virtual void setAtk(int atk) = 0;
    
    virtual int getCurFlyingDist() = 0;
    virtual void incCurFlyingDist() = 0;
    virtual void resetCurFlyingDist() = 0;
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// 3rd Generation
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

class Potion : public ConsumableItem {
protected:
    int hp;
    
public:
    Potion();
    Potion(Potion &other);
    Potion(int hp);
    virtual ~Potion();
    
    bool isPotion();
    
    virtual bool use(Unit *unit);
    
    Item *clone();
};

class Dagger : public ThrowableItem {
protected:
    int numTicksWhenMoved;
    int range;
    int atk;
    int curFlyingDist; // 0 to range
    
public:
    Dagger();
    Dagger(Dagger &other);
    Dagger(int range, int atk);
    virtual ~Dagger();
    
    int getNumTicksWhenMoved();
    void setNumTicksWhenMoved(int numTicksWhenMoved);

    int getRange();
    void setRange(int range);
    
    int getAtk();
    void setAtk(int atk);

    int getCurFlyingDist();
    void incCurFlyingDist();
    void resetCurFlyingDist();
    
    virtual bool use(Unit *unit);
    
    Item *clone();
};

class Armor : public EquippableItem {
protected:
    int def;
    int bodyPartID;
public:
    Armor();
    Armor(Armor &other);
    Armor(char shape, string name, int price, bool disposable, bool removable, int durability, int def, int bodyPartID);
    virtual ~Armor();

    virtual void printFullSpec();

    int getDef();
    int getBodyPartID();

    bool isArmor();
    
    bool use(Unit *unit);
    
    Item *clone();
};

Armor *createRandomArmor();
Armor *getRandomArmorByName(string name);
Armor *getRandomArmor(int pBodyPartID);

class Weapon : public EquippableItem {
protected:
    int atk;
    int numHands;
public:
    Weapon();
    Weapon(Weapon &other);
    Weapon(char shape, string name, int price, bool disposable, bool removable, int durability, int atk, int numHands);
    virtual ~Weapon();
    
    virtual void printFullSpec();

    int getAtk();
    int getNumHands();

    bool isWeapon();
    
    bool use(Unit *unit);
    
    Item *clone();
};

Weapon *createRandomWeapon();
Weapon *getRandomWeaponByName(string name);
Weapon *getRandomWeapon(int pNumHands);

#endif
