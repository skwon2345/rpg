#include <iostream>

using namespace std;

#include "main.h"
#include "magic.h"
#include "magicbook.h"

Magicbook::Magicbook() {
    init(MAGICBOOK_MAX_NUM_MAGICS);
}

Magicbook::Magicbook(int maxNumMagics) {
    init(maxNumMagics);
}

void Magicbook::init(int maxNumMagics) {
    this->maxNumMagics = maxNumMagics;
    
    magics = new Magic *[maxNumMagics]; // Item *를(Item object 한개의 주소) 담는 1D array 만듦.
    
    // 중간에 NULL이 있는 slot이 빈 칸으로 간주되므로, 꼭 NULL을 넣어야 한다. 나중에 지울때도, 그 object를 delete하고 난 후, 그 자리에 NULL을 넣어야 한다.
    for (int i = 0; i < maxNumMagics; i++) {
        magics[i] = NULL;
    }
    
    numMagics = 0;

}

void Magicbook::reset() {
    for (int i = 0; i < maxNumMagics; i++) {
        if (magics[i] != NULL) {
            delete magics[i];
            magics[i] = NULL;
        }
    }
    
    numMagics = 0;
}

Magicbook::~Magicbook() {
    for (int i = 0; i < maxNumMagics; i++) {
        if (magics[i] != NULL) {
            delete magics[i];
            magics[i] = NULL;
        }
    }
    
    delete [] magics;
    magics = NULL;
    
    numMagics = 0;
}

void Magicbook::print() {
    cout << "[";
    for (int i = 0; i < maxNumMagics; i++) {
        if (magics[i] == NULL) {
            cout << " ";
        }
        else {
            magics[i]->print();
        }
        
        if (i < maxNumMagics-1) {
            cout << "|";
        }
    }
    cout << "]";
}

//  1 2 3 4 5 6
// [b| |*| | | ]
void Magicbook::printIndices(int startNumber) {
    cout << " ";
    for (int i = 0; i < maxNumMagics; i++) {
        cout << ((i+startNumber)%10);
        
        if (i < maxNumMagics-1) {
            cout << " ";
        }
    }
    cout << " ";
}

bool Magicbook::add(Magic *magic) {
    if (magic == NULL) { // validate
        return false;
    }
    
    if (isFull()) {
        return false;
    }
    
    for (int i = 0; i < maxNumMagics; i++) {
        if (magics[i] == NULL) {
            magics[i] = magic;
            numMagics++;
            break;
        }
    }
    
    return true;
}

bool Magicbook::insertMagicAt(Magic *magic, int index) {
    if (magic == NULL) { // validate
        return false;
    }
    
    // 사실상 아래의 코드는 개발할 때만 쓰고, 출시할 때는 지우는 것이 좋다. 일어나지 말아야 할 일. 하지만 개발도중에는 초보 프로그래머가 충분히 저지를 수 있는 일.
    if (index < 0 || index >= maxNumMagics) {
        cout << "error: Magicbook::insertMagicAt(): if (index < 0 || index >= maxNumMagics) {" << endl;
        exit(1);
    }
    
    if (magics[index] == NULL) {
        magics[index] = magic;
        numMagics++;
        return true;
    }
    else {
        return false;
    }
}

Magic *Magicbook::getMagicAt(int index) {
    if (index < 0 || index >= maxNumMagics) {
        cout << "error: Magicbook::getMagicAt(): if (index < 0 || index >= maxNumMagics) {" << endl;
        exit(1);
    }
    
    return magics[index];

}

Magic *Magicbook::removeMagicAt(int index) {
    if (index < 0 || index >= maxNumMagics) {
        cout << "error: Magicbook::removeMagicAt(): if (index < 0 || index >= maxNumMagics) {" << endl;
        exit(1);
    }
    
    if (magics[index] != NULL) {
        Magic *magicToRemove = magics[index];
        magics[index] = NULL;
        numMagics--;
        return magicToRemove;
    }
    else {
        return NULL;
    }
}

void Magicbook::castMagicAt(int index, Unit *unit) {
    if (index < 0 || index >= maxNumMagics) {
        cout << "error: Magicbook::effectMagicAt(): if (index < 0 || index >= maxNumMagics) {" << endl;
        exit(1);
    }
    
    if (unit == NULL) {
        cout << "error: Magicbook::effectMagicAt(): if (unit == NULL) {" << endl;
        exit(1);
    }
    
    if (magics[index] != NULL) {
        if (magics[index]->castMagic(unit)) {
            // 성공적으로 사용되어졌을 경우..

            // item과 다르게 magic은 RPG에서 보통 mana가 있는한 무제한이거나, 약초 셋가 있어야 그만큼 casting할 수 있게 되있고, 어쨌든 지워지지는 않는다.
            // 나중에는 slot처럼 따로 casting을 위한 slot을 만들어서, magic book으로부터 거기에 필요한 것만 넣은 다음, casting한다.
//            delete magics[index];
//            magics[index] = NULL;
//            numMagics--;
        }
    }

}

void Magicbook::effectBySkillNumber(Unit *unit, Item *item, Prop *prop, int skillNumber, Unit *user) {
    for (int i = 0; i < maxNumMagics; i++) {
        if (magics[i] != NULL && magics[i]->getSkillNumber() == skillNumber) {
            magics[i]->effect(unit, item, prop, user);
        }
    }
}

bool Magicbook::isFull() {
    return numMagics >= maxNumMagics;

}

int Magicbook::size() {
    return numMagics;
}

int Magicbook::capacity() {
    return maxNumMagics;
}
