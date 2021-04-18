#include <iostream>

using namespace std;

#include "main.h"
#include "item.h"
#include "unit.h"
#include "paperdoll.h"

Paperdoll::Paperdoll() {
    head = NULL;
    chest = NULL;
    gloves = NULL;
    back = NULL;
    legs = NULL;
    feet = NULL;
    leftHand = NULL;
    rightHand = NULL;
}

void Paperdoll::reset() {
    destroy();
}

Paperdoll::~Paperdoll() {
    destroy();
}

void Paperdoll::destroy() {
    if (head != NULL) {
        delete head;
        head = NULL;
    }
    if (chest != NULL) {
        delete chest;
        chest = NULL;
    }
    if (gloves != NULL) {
        delete gloves;
        gloves = NULL;
    }
    if (back != NULL) {
        delete back;
        back = NULL;
    }
    if (legs != NULL) {
        delete legs;
        legs = NULL;
    }
    if (feet != NULL) {
        delete feet;
        feet = NULL;
    }
    if (leftHand != NULL) {
        delete leftHand;
        leftHand = NULL;
    }
    if (rightHand != NULL) {
        delete rightHand;
        rightHand = NULL;
    }
}

void Paperdoll::print() {
    cout << "0:HEAD[" << ((head == NULL)?' ':head->getShape()) << "] ";
    cout << "1:CHEST[" << ((chest == NULL)?' ':chest->getShape()) << "] ";
    cout << "2:GLOVES[" << ((gloves == NULL)?' ':gloves->getShape()) << "] ";
    cout << "3:BACK[" << ((back == NULL)?' ':back->getShape()) << "] ";
    cout << "4:LEGS[" << ((legs == NULL)?' ':legs->getShape()) << "] ";
    cout << "5:FEET[" << ((feet == NULL)?' ':feet->getShape()) << "] ";
    cout << "6:LH[" << ((leftHand == NULL)?' ':leftHand->getShape()) << "] ";
    cout << "7:RH[" << ((rightHand == NULL)?' ':rightHand->getShape()) << "]";
}

bool Paperdoll::equip(Unit *unit, Item *itemToEquip) {
    if (unit == NULL) {
        cout << "Paperdoll::equip(): if (unit == NULL) {" << endl;
        exit(1);
    }
    
    if (itemToEquip == NULL) {
        cout << "Paperdoll::equip(): if (itemToEquip == NULL) {" << endl;
        exit(1);
    }
    
    if (itemToEquip->isEquippable()) {
        EquippableItem *equippableItem = (EquippableItem *)itemToEquip;
        
        if (equippableItem->isArmor()) {
            Armor *armor = (Armor *)equippableItem;
            
            int bodyPartID = armor->getBodyPartID();
            if (bodyPartID == ARMOR_BODYPART_ID_HEAD) {
                if (head == NULL) {
                    head = armor;
                    unit->incDef(armor->getDef());
                    return true;
                }
                else {
                    return false;
                }
            }
            else if (bodyPartID == ARMOR_BODYPART_ID_CHEST) {
                if (chest == NULL) {
                    chest = armor;
                    unit->incDef(armor->getDef());
                    return true;
                }
                else {
                    return false;
                }
            }
            else if (bodyPartID == ARMOR_BODYPART_ID_GLOVES) {
                if (gloves == NULL) {
                    gloves = armor;
                    unit->incDef(armor->getDef());
                    return true;
                }
                else {
                    return false;
                }
            }
            else if (bodyPartID == ARMOR_BODYPART_ID_BACK) {
                if (back == NULL) {
                    back = armor;
                    unit->incDef(armor->getDef());
                    return true;
                }
                else {
                    return false;
                }
            }
            else if (bodyPartID == ARMOR_BODYPART_ID_LEGS) {
                if (legs == NULL) {
                    legs = armor;
                    unit->incDef(armor->getDef());
                    return true;
                }
                else {
                    return false;
                }
            }
            else if (bodyPartID == ARMOR_BODYPART_ID_FEET) {
                if (legs == NULL) {
                    legs = armor;
                    unit->incDef(armor->getDef());
                    return true;
                }
                else {
                    return false;
                }
            }

            // 숙제.. 나머지 부분.. 그냥 복붙............................................................. 부츠까지(손을 빼고)
        }
        else if (equippableItem->isWeapon()) {
            Weapon *weapon = (Weapon *)equippableItem;

            if (weapon->getNumHands() == WEAPON_ONE_HAND) {
                if (rightHand == NULL) {
                    rightHand = weapon;
                    unit->incAtk(weapon->getAtk());
                    return true;
                }
                else if (leftHand == NULL) {
                    if (rightHand->getNumHands() == WEAPON_ONE_HAND) {
                        leftHand = weapon;
                        unit->incAtk(weapon->getAtk());
                        return true;
                    }
                    else {
                        return false;
                    }
                }
                else {
                    return false;
                }
            }
            else { // if (weapon->getNumHands() == WEAPON_TWO_HAND) {
                if (rightHand == NULL) {
                    rightHand = weapon;
                    unit->incAtk(weapon->getAtk());
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else {
            cout << "Paperdoll::equip(): unsupported type item!!!" << endl;
            exit(1);
        }
    }

    return false;
}

//#define ARMOR_BODYPART_ID_NONE -1
//#define ARMOR_BODYPART_ID_HEAD 0
//#define ARMOR_BODYPART_ID_CHEST 1
//#define ARMOR_BODYPART_ID_GLOVES 2
//#define ARMOR_BODYPART_ID_BACK 3
//#define ARMOR_BODYPART_ID_PANTS 4
//#define ARMOR_BODYPART_ID_BOOTS 5
//#define WEAPON_BODYPART_ID_LEFT_HAND 6
//#define WEAPON_BODYPART_ID_RIGHT_HAND 7

Item *Paperdoll::unequip(Unit *unit, int bodyPartID) {
    if (unit == NULL) {
        cout << "Paperdoll::unequip(): if (unit == NULL) {" << endl;
        exit(1);
    }
    
    if (bodyPartID < 0 || bodyPartID > WEAPON_BODYPART_ID_RIGHT_HAND) {
        cout << "Paperdoll::unequip(): if (bodyPartID < 0 || bodyPartID > WEAPON_BODYPART_ID_RIGHT_HAND) {" << endl;
        exit(1);
    }
    
    if (bodyPartID == ARMOR_BODYPART_ID_HEAD) {
        Item *itemToReturn = head;
        if (itemToReturn != NULL) {
            unit->decDef(((Armor *)itemToReturn)->getDef());
        }
        head = NULL;
        return itemToReturn;
    }
    else if (bodyPartID == ARMOR_BODYPART_ID_CHEST) {
        Item *itemToReturn = chest;
        if (itemToReturn != NULL) {
            unit->decDef(((Armor *)itemToReturn)->getDef());
        }
        chest = NULL;
        return itemToReturn;
    }
    else if (bodyPartID == ARMOR_BODYPART_ID_GLOVES) {
        Item *itemToReturn = gloves;
        if (itemToReturn != NULL) {
            unit->decDef(((Armor *)itemToReturn)->getDef());
        }
        gloves = NULL;
        return itemToReturn;
    }
    else if (bodyPartID == ARMOR_BODYPART_ID_BACK) {
        Item *itemToReturn = back;
        if (itemToReturn != NULL) {
            unit->decDef(((Armor *)itemToReturn)->getDef());
        }
        back = NULL;
        return itemToReturn;
    }
    else if (bodyPartID == ARMOR_BODYPART_ID_LEGS) {
        Item *itemToReturn = legs;
        if (itemToReturn != NULL) {
            unit->decDef(((Armor *)itemToReturn)->getDef());
        }
        legs = NULL;
        return itemToReturn;
    }
    else if (bodyPartID == ARMOR_BODYPART_ID_FEET) {
        Item *itemToReturn = feet;
        if (itemToReturn != NULL) {
            unit->decDef(((Armor *)itemToReturn)->getDef());
        }
        feet = NULL;
        return itemToReturn;
    }
    else if (bodyPartID == WEAPON_BODYPART_ID_LEFT_HAND) {
        Item *itemToReturn = leftHand;
        if (itemToReturn != NULL) {
            unit->decAtk(((Weapon *)itemToReturn)->getAtk());
        }
        leftHand = NULL;
        return itemToReturn;
    }
    else if (bodyPartID == WEAPON_BODYPART_ID_RIGHT_HAND) {
        Item *itemToReturn = rightHand;
        if (itemToReturn != NULL) {
            unit->decAtk(((Weapon *)itemToReturn)->getAtk());
        }
        rightHand = leftHand;
        leftHand = NULL;
        return itemToReturn;
    }
    
    return NULL;
}
