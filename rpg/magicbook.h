#ifndef MAGICBOOK_H
#define MAGICBOOK_H

class Magic;
class Unit;
class Item;
class Prop;

class Magicbook {
private:
    Magic **magics;
    int maxNumMagics;
    int numMagics;
public:
    Magicbook();
    Magicbook(int maxNumMagics);
    void init(int maxNumMagics);
    virtual void reset();

    virtual ~Magicbook();
    
    void print();
    void printIndices(int startNumber);
    
    bool add(Magic *magic); // 빈곳에 넣는다.
    bool insertMagicAt(Magic *magic, int index); // given index에 넣어야 함.
    
    Magic *getMagicAt(int index);
    Magic *removeMagicAt(int index); // given index의 item을 return한다. 단 그 자리에 NULL을 넣는다.
    
    void castMagicAt(int index, Unit *unit); // given index의 item을 use하고, 만약 그 아이템이 disposable이면 delete한다. 그 아이템이 removable(equippable)이면 delete(heap에서)하지는 않고, inventory에서만 빠진다.
    void effectBySkillNumber(Unit *unit, Item *item, Prop *prop, int skillNumber, Unit *user);

    bool isFull();
    
    int size();
    int capacity();
};

#endif
