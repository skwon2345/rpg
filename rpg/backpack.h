#ifndef BACKPACK_H
#define BACKPACK_H

class BackpackNode;
class Item;
class Inventory;

class Backpack {
protected:
    BackpackNode *head;
    
    int size;
public:
    Backpack();
    Backpack(BackpackNode *head);
    virtual void reset();

    virtual ~Backpack();
    
    void print();
    void printIndices(int startNumber);

    void insert(Item *item);
    
    Item *removeItemAt(int index);
    
    void moveFromBackpackTo(Inventory *slot);
    
    void moveToBackpackFrom(Inventory *slot);
};

#endif
