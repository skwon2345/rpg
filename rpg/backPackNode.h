#ifndef BACKPACKNODE_H
#define BACKPACKNODE_H

#include "main.h"
#include "item.h"

class Item;

class BackpackNode {
protected:
public:
    Item *item;
    BackpackNode* next;
    
    BackpackNode();
    BackpackNode(Item *item, BackpackNode* next);
    virtual ~BackpackNode();
    
    void print();
};
#endif
