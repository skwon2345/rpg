#include <iostream>

using namespace std;

#include "main.h"
#include "unit.h"
#include "item.h"
#include "hero.h"
#include "inventory.h"
#include "merchant.h"
#include "sim.h"
#include "statistics.h"

extern Sim *sim;

Merchant::Merchant() : Unit(SHAPE_MERCHANT, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0) {
    itemsToSell = new Inventory(MAX_LEN_ITEMS_TO_SELL);
    itemsToBuy = new Inventory(MAX_LEN_ITEMS_BOUGHT);
    
    for (int i = 0; i < itemsToSell->capacity(); i++) {
        itemsToSell->add(createRandomItem());
    }
}

Merchant::Merchant(char shape, int row, int col, int hp, int maxHp, int mp, int maxMp, int dir, int atk, int def, int gold, int exp) : Unit(shape, row, col, hp, maxHp, mp, maxMp, dir, atk, def, gold, exp) {
    itemsToSell = new Inventory();
    itemsToBuy = new Inventory();
    
    for (int i = 0; i < itemsToSell->capacity(); i++) {
        itemsToSell->add(createRandomItem());
    }
}

void Merchant::reset() {
    Unit::reset();
    
    itemsToSell->reset();
    for (int i = 0; i < itemsToSell->capacity(); i++) {
        itemsToSell->add(createRandomItem());
    }
    
    itemsToBuy->reset();
}

Merchant::~Merchant() {
    if (itemsToSell != NULL) {
        delete itemsToSell;
        itemsToSell = NULL;
    }
    
    if (itemsToBuy != NULL) {
        delete itemsToBuy;
        itemsToBuy = NULL;
    }
}

void Merchant::printItems() {
    cout << "Merchant: ";
    itemsToSell->print();
    cout << endl;
    cout << "Hero: ";
    itemsToBuy->print();
    cout << endl;
}

bool Merchant::isMerchant() {
    return true;
}

//bool Merchant::sellItemAt(int index, Unit *unit) {
//    if (index < 0) {
//        for (int i = 0; i < itemsToBuy->getMaxNumItems(); i++) {
//            if (!((Hero*)unit)->getSlot()->isFull()) {
//                ((Hero*)unit)->getSlot()->add(itemsToBuy->getItemAt(i));
//            }
//            itemsToBuy->removeItemAt(i);
//        }
//
//        return false;
//    }
//    if (unit->getGold() >= itemsToSell->getItemAt(index)->getPrice()) {
//        unit->decGold(itemsToSell->getItemAt(index)->getPrice());
//        itemsToBuy->add(itemsToSell->getItemAt(index));
//    }
//    return true;
//}

void Merchant::move(int dir) {
    return;
}

void Merchant::interact(Unit *unit) {
    while (true) {
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "------------------------- Suk-Kwon's Store ------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        
        char command = '\0';
        
        cout << "(B)uy, (S)ell, or e(X)it: ";
        cin >> command;
        
        if (command == 'b' || command == 'B') {
            buy(unit);
        }
        else if (command == 's' || command == 'S') {
            sell(unit);
        }
        else if (command == 'x' || command == 'X') {
            break;
        }
        else {
            cout << "error: No such command!!" << endl;
        }
    }
}

void Merchant::buy(Unit *unit) {
    while (true) {
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "------------------ Suk-Kwon's Store: B U Y ------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        
        for (int i = 0; i < itemsToSell->capacity(); i++) {
            cout << ((i/10 == 0)?' ':(char)((i/10)+'0')) << (i%10) << ": ";
            if (itemsToSell->getItemAt(i) != NULL) {
                itemsToSell->getItemAt(i)->printFullSpec();
            }
            else {
                cout << "[NONE]";
            }
            cout << endl;
        }
        cout << "..................................................................." << endl;
        unit->printStat();
        cout << "..................................................................." << endl;
        
        int itemIndexToBuy = -1;
        
        cout << "Enter the index number to buy(0 to " << (itemsToSell->size()-1) << ", or -1 to exit): ";
        cin >> itemIndexToBuy;
        
        if (itemIndexToBuy >= 0 && itemIndexToBuy < itemsToSell->size()) {
            if (itemsToSell->getItemAt(itemIndexToBuy) != NULL) {
                if (unit->canReceiveItem()) {
                    Item *itemToBuy = itemsToSell->getItemAt(itemIndexToBuy);
                    if (itemToBuy->getPrice() <= unit->getGold()) {
                        unit->decGold(itemToBuy->getPrice());
                        sim->statistics->totalMoneySpentHero += gold;
                        unit->receiveItem(itemToBuy->clone());
                    }
                    else {
                        cout << "error: you can't afford!" << endl;
                    }
                }
                else {
                    cout << "error: your inventory is full!" << endl;
                }
            }
            else {
                cout << "error: the slot is empty!" << endl;
                
            }
        }
        else if (itemIndexToBuy == -1) {
            break;
        }
        else {
            cout << "error: invalid input!!" << endl;
        }
    }
}

void Merchant::sell(Unit *unit) {
    Inventory *temp = new Inventory(itemsToBuy->capacity());
    int size = ((Hero*)unit)->getSlot()->size();
    int capacity = ((Hero*)unit)->getSlot()->capacity();
    
    while (true) {
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "------------------ Suk-Kwon's Store: S E L L ----------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        cout << "-------------------------------------------------------------------" << endl;
        
        for (int i = 0; i < ((Hero*)unit)->getSlot()->capacity(); i++) {
            cout << ((i/10 == 0)?' ':(char)((i/10)+'0')) << (i%10) << ": ";
            if (((Hero*)unit)->getSlot()->getItemAt(i) != NULL) {
                ((Hero*)unit)->getSlot()->getItemAt(i)->printFullSpec();
            }
            else {
                cout << "[NONE]";
            }
            cout << endl;
        }
        
        cout << "..................................................................." << endl;
        cout << "Items to sell Merchant";
        temp->print();
        cout << endl;
        cout << "..................................................................." << endl;
        
        for (int i = 0; i < itemsToBuy->capacity(); i++) {
            cout << ((i/10 == 0)?' ':(char)((i/10)+'0')) << (i%10) << ": ";
            if (itemsToBuy->getItemAt(i) != NULL) {
                itemsToBuy->getItemAt(i)->printFullSpec();
            }
            else {
                cout << "[NONE]";
            }
            cout << endl;
        }
        
        int itemIndexToSell = -1;
        
        if (((Hero*)unit)->getSlot()->size() >0) {
            cout << "Enter the index number to sell(0 to " << (capacity-1) << ", 9 to cancel, or -1 to exit): ";
        }
        
        else {
            cout << "Enter -1 to exit): ";
        }
        
        cin >> itemIndexToSell;
        
        if (itemIndexToSell >= 0 && itemIndexToSell < capacity) {
            if (((Hero*)unit)->getSlot()->getItemAt(itemIndexToSell) != NULL) {
                if (itemsToBuy->size() + temp->size() <= itemsToBuy->capacity()) {
                    Item *itemToSell = ((Hero*)unit)->getSlot()->getItemAt(itemIndexToSell);
                    unit->incGold(itemToSell->getPrice());
                    unit->sellItem(temp, itemIndexToSell);
                }
                else {
                    cout << "error: merchant is full!" << endl;
                }
            }
            else {
                cout << "error: the slot is empty!" << endl;
            }
        }
        else if (itemIndexToSell == -1) {
            if (temp->size() > 0) {
                temp->size();
                for (int i = 0; i < temp->size(); i++) {
                    itemsToBuy->add(temp->getItemAt(i));
                }
            }
            delete temp;
            
            break;
        }
        else if (itemIndexToSell == 9) {
            if (temp->size() > 0) {
                unit->cancelToSellLastItem(temp);
            }
            else {
                cout << "No Item to cancel." << endl;
            }
        }
        else {
            cout << "error: invalid input!!" << endl;
        }
    }
}
