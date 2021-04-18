#include <iostream>

using namespace std;

#include "main.h"
#include "item.h"
#include "unit.h"
#include "inventory.h"
#include "event.h"
#include "priority_queue.h"

#include "sim.h"
extern Sim *sim;

Inventory::Inventory() {
    init(INVENTORY_MAX_NUM_ITEMS);
}

Inventory::Inventory(int maxNumItems) {
    init(maxNumItems);
}

void Inventory::init(int maxNumItems) {
    this->maxNumItems = maxNumItems;
    
    items = new Item *[maxNumItems]; // Item *를(Item object 한개의 주소) 담는 1D array 만듦.
    
    // 중간에 NULL이 있는 slot이 빈 칸으로 간주되므로, 꼭 NULL을 넣어야 한다. 나중에 지울때도, 그 object를 delete하고 난 후, 그 자리에 NULL을 넣어야 한다.
    for (int i = 0; i < maxNumItems; i++) {
        items[i] = NULL;
    }
    
    numItems = 0;
}

Inventory::Inventory(Inventory &other) {
    this->maxNumItems = other.maxNumItems;
    
    items = new Item *[maxNumItems];
    
    for (int i = 0; i < maxNumItems; i++) {
        items[i] = other.getItemAt(i);
    }
    
    numItems = other.numItems;
}

void Inventory::reset() {
    for (int i = 0; i < maxNumItems; i++) {
        if (items[i] != NULL) {
            delete items[i];
            items[i] = NULL;
        }
    }
    numItems = 0;
}

Inventory::~Inventory() {
    for (int i = 0; i < maxNumItems; i++) {
        if (items[i] != NULL) {
            delete items[i];
            items[i] = NULL;
        }
    }
    
    delete [] items;
    items = NULL;
    
    numItems = 0;
}

// [b| |*| | | ]
void Inventory::print() {
    cout << "[";
    for (int i = 0; i < maxNumItems; i++) {
        if (items[i] == NULL) {
            cout << " ";
        }
        else {
            items[i]->print();
        }
        
        if (i < maxNumItems-1) {
            cout << "|";
        }
    }
    cout << "]";
}

//  1 2 3 4 5 6
// [b| |*| | | ]
void Inventory::printIndices(int startNumber) {
    cout << " ";
    for (int i = 0; i < maxNumItems; i++) {
        cout << ((i+startNumber)%10);
        
        if (i < maxNumItems-1) {
            cout << " ";
        }
    }
    cout << " ";
}

//int Inventory::getMaxNumItems() {
//    return maxNumItems;
//}
//int Inventory::getNumItems() {
//    return numItems;
//}

//--------------------------------------------------------------------------
// CRUD: Create, Read, Update, Delete
//--------------------------------------------------------------------------

bool Inventory::add(Item *item) { // 빈곳에 넣는다.
    if (item == NULL) { // validate
        return false;
    }
    
    if (isFull()) {
        return false;
    }
    
    for (int i = 0; i < maxNumItems; i++) {
        if (items[i] == NULL) {
            items[i] = item;
            numItems++;
            break;
        }
    }
    
    return true;
}

bool Inventory::insertItemAt(Item *item, int index) { // given index에 넣어야 함.
    if (item == NULL) { // validate
        return false;
    }

    // 사실상 아래의 코드는 개발할 때만 쓰고, 출시할 때는 지우는 것이 좋다. 일어나지 말아야 할 일. 하지만 개발도중에는 초보 프로그래머가 충분히 저지를 수 있는 일.
    if (index < 0 || index >= maxNumItems) {
        cout << "error: Inventory::insertItemAt(): if (index < 0 || index >= maxNumItems) {" << endl;
        exit(1);
    }
    
    if (items[index] == NULL) {
        items[index] = item;
        numItems++;
        return true;
    }
    else {
        return false;
    }
}

Item *Inventory::getItemAt(int index) {
    if (index < 0 || index >= maxNumItems) {
        cout << "error: Inventory::getItemAt(): if (index < 0 || index >= maxNumItems) {" << endl;
        exit(1);
    }
    
    return items[index];
}

Item *Inventory::removeItemAt(int index) { // given index의 item을 return한다. 단 그 자리에 NULL을 넣는다.
    if (index < 0 || index >= maxNumItems) {
        cout << "error: Inventory::removeItemAt(): if (index < 0 || index >= maxNumItems) {" << endl;
        exit(1);
    }
    
    if (items[index] != NULL) {
        Item *itemToRemove = items[index];
        items[index] = NULL;
        numItems--;
        return itemToRemove;
    }
    else {
        return NULL;
    }
}

void Inventory::useItemAt(int index, Unit *unit) { // given index의 item을 use하고, 만약 그 아이템이 disposable이면 delete한다. 그 아이템이 removable(equippable)이면 delete(heap에서)하지는 않고, inventory에서만 빠진다.
    if (index < 0 || index >= maxNumItems) {
        cout << "error: Inventory::useItemAt(): if (index < 0 || index >= maxNumItems) {" << endl;
        exit(1);
    }
    
    if (unit == NULL) {
        cout << "error: Inventory::useItemAt(): if (unit == NULL) {" << endl;
        exit(1);
    }
    
    if (items[index] != NULL) {
        if (items[index]->use(unit)) {
            // 성공적으로 사용되어졌을 경우..
            if (items[index]->isRemovable()) {
                if (items[index]->isRemoved()) {
                    items[index] = NULL;
                    numItems--;
                }
            }
            else if (items[index]->isDisposable()) {
                delete items[index];
                items[index] = NULL;
                numItems--;
            }
        }
    }
}

bool Inventory::isFull() {
    return numItems >= maxNumItems;
}

int Inventory::size() {
    return numItems;
}

int Inventory::capacity() {
    return maxNumItems;
}
