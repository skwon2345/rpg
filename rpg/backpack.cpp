#include <iostream>

using namespace std;

#include "backpack.h"
#include "backPackNode.h"
#include "inventory.h"

Backpack::Backpack() {
    head = NULL;
    size = 0;
}

Backpack::Backpack(BackpackNode *head) {
    this->head = head;
    size = 1;
}

void Backpack::reset() {
    while (head != NULL) {
        BackpackNode *temp = head->next; //이걸 지우고
        //        temp = head->next;
        delete head;
        head = temp;
    }
    
    //    delete temp;
    //    delete head;
    //    head = NULL;
    size = 0;
}

Backpack::~Backpack() {
//    BackpackNode *temp = NULL;
    while (head != NULL) {
        BackpackNode *temp = head->next; //이걸 지우고
//        temp = head->next;
        delete head;
        head = temp;
    }
//    delete temp;
//    delete head;
//    head = NULL;
    size = 0;
}

void Backpack::print() {
    BackpackNode *temp = head;
    
    cout << "[";
    for (int i = 0; i < size; i++) {
        temp->print();
        temp = temp->next;
        if (temp != NULL) {
            cout << "|";
        }
    }
    cout << "]";
}

void Backpack::printIndices(int startNumber) {
    BackpackNode *temp = head;
    
    cout << " ";
    for (int i = 0; i < size; i++) {
        cout << ((i+startNumber)%10);
        temp = temp->next;
        if (temp != NULL) {
            cout << " ";
        }
    }
    cout << " ";
}

void Backpack::insert(Item *item) {
    if (head == NULL) {
        head = new BackpackNode(item, NULL);
        size++;
        return;
    }
    
    BackpackNode *temp = head;
    
    while (temp->next != NULL) {
        temp = temp->next;
    }
    
    temp->next = new BackpackNode(item, NULL);
    size++;
}

Item *Backpack::removeItemAt(int index) {
    BackpackNode *toBeRemoved = head;
    Item *toBeReturned = NULL;
    
    if (index == 0) {
        head = head->next;
        toBeReturned = toBeRemoved->item->clone();
        delete toBeRemoved;
        size--;
    }
    
    else if (index > 0) {
        BackpackNode *prev = head;
        
        for (int i = 0; i < index; i++) {
            toBeRemoved = toBeRemoved->next;
        }
        while (prev->next != toBeRemoved) {
            prev = prev->next;
        }
        
        prev->next = toBeRemoved->next;
        toBeReturned = toBeRemoved->item->clone();
        delete toBeRemoved;
        size--;
    }
    
    else {
        cout << "Backpack::removeItemAt() : error - index input < 0" << endl;
        exit(1);
    }
    
    return toBeReturned;
}

void Backpack::moveFromBackpackTo(Inventory *slot) {
    while (true) {
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "------------------ Suk-Kwon's Backpack : F R O M ------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        
        cout << endl;
        
        int command = -1;
        
        cout << "           ";
        printIndices(0);
        cout << endl;
        cout << "Backpack : ";
        print();
        cout << endl;
        cout << endl;

        cout << "Slot : ";
        slot->print();
        cout << endl;
        cout << endl;

        cout << "Press 0 to " << size - 1 << " to move an item from backpack to slot or -1 to exit: ";
        cin >> command;
        
        if (command < 0 || command >= size) {
            if (command == -1) {
                break;
            }
            
            cout << "This index is not available at this point." << endl;
        }
        
        else if (command >= 0 && command < size) {
            if (slot->isFull()) {
                cout << "Slot is Full." << endl;
            }
            else {
                slot->add(removeItemAt(command));
            }
        }
        
        else {
            cout << "Invalid input." << endl;
        }
    }
}

void Backpack::moveToBackpackFrom(Inventory *slot) {
    while (true) {
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "--------------------- Suk-Kwon's Backpack : T O -------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        
        cout << endl;
        
        int command = -1;
        
        cout << "Backpack : ";
        print();
        cout << endl;
        cout << endl;

        cout << "           ";
        slot->printIndices(0);
        cout << endl;
        cout << "Slot     : ";
        slot->print();
        cout << endl;
        cout << endl;

        cout << "Press 0 to " << slot->capacity() - 1 << " to move an item from backpack to slot or -1 to exit: ";
        cin >> command;
        
        if (command < 0 || command >= slot->capacity()) {
            if (command == -1) {
                break;
            }
            
            cout << "This index is not available at this point." << endl;
        }
        
        else if (command >= 0 && command < slot->capacity()) {
            if (slot->getItemAt(command) == NULL) {
                cout << "No item for this index." << endl;
            }
            else {
                insert(slot->removeItemAt(command));
            }
        }
        
        else {
            cout << "Invalid input." << endl;
        }


    }
}


