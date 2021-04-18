#ifndef MAGIC_H
#define MAGIC_H

#include <iostream>

using namespace std;

class Unit;
class Item;
class Prop;

class Magic{
protected:
    char shape;
    
    int skillNumber;
    string name;
    
    int atk;
    int delay;
    int range;
    
    int mp; //???????????????????????????????????????????????????????????????? 추가할 것.
    
public:
    Magic();
    Magic(char shape, int skillNumber, string name, int atk, int delay, int range, int mp);
    virtual ~Magic();
    
    string getName();
    int getSkillNumber();
    
    void print();
    
    virtual bool castMagic(Unit *unit) = 0;
    virtual void effect(Unit *unit, Item *item, Prop *prop, Unit *user) = 0;
};

class Fireball : public Magic {
protected:
public:
    Fireball();
    Fireball(char shape, int skillNumber, string name, int atk, int delay, int range, int mp);
    virtual ~Fireball();
    
    void print();
    
    virtual bool castMagic(Unit *unit);
    virtual void effect(Unit *unit, Item *item, Prop *prop, Unit *user);
};

#endif
