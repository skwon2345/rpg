#include <iostream>

using namespace std;

#include "backPackNode.h"
#include "item.h"

BackpackNode::BackpackNode() {
    item = NULL;
    next = NULL;
}

BackpackNode::BackpackNode(Item *item,BackpackNode* next) {
    this->item = item;
    this->next = next;
}

BackpackNode::~BackpackNode() {
    if (item != NULL) {
        delete item;
        item = NULL;
    }
    //next는?
}

void BackpackNode::print() {
    item->print();
}
