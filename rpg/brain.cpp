#include <iostream>
#include <vector>

using namespace std;

#include "main.h"
#include "brain.h"
#include "unit.h"
#include "prop.h"
#include "board.h"
#include "hero.h"
#include "pet.h"
#include "inventory.h"
#include "magicbook.h"
#include "ridable.h"
#include "pos.h"
#include "node.h"
#include "list.h"
#include "monster.h"
#include "tile.h"

//extern Board *board;
#include "sim.h"
extern Sim *sim;

#define IQ_NONE 0

Brain::Brain() {
    owner = NULL;
    iq = IQ_100;
    path[0] = NULL;
    path[1] = NULL;
    pathFound = false;
    pathLen = 0;
    curIndexPath = -1;
}

//int *path[2]; // row, ownerCol을 위해 2개의 row는 그대로 두고, ownerCol길이는 자유롭게 풀었다. 왜냐하면 IQ나 memory크기에 따라서 달라지므로..
//bool pathFound;
//int pathMaxLen; // memory용량. 클수록 긴거리까지 추적가능.
//int pathLen;
//int curIndexPath;
//int range;

Brain::Brain(Unit *owner) {
    this->owner = owner;
    iq = IQ_100; // 기본적인 IQ는 일단 100정도로 하지만, 만약 자식 class가 IQ120이라면 알아서 이 constructor후에 다시 IQ_120을 넣게 된다.
    path[0] = NULL;
    path[1] = NULL;
    pathFound = false;
    pathLen = 0;
    curIndexPath = -1;
}

void Brain::init() {
//    if (owner == NULL) {
//        iq = IQ_NONE;
//        cout << "Brain::Brain(): iq = IQ_NONE" << endl;
//    }
//    else if (owner->isHero()) {
//        iq = HERO_IQ;
//        cout << "Brain::Brain(): iq = HERO_IQ" << endl;
//    }
//    else if (owner->isMonster() && !owner->isBoss()) {
//        iq = MONSTER_IQ;
//        cout << "Brain::Brain(): iq = MONSTER_IQ" << endl;
//    }
//    else if (owner->isBoss()) {
//        iq = BOSS_IQ;
//        cout << "Brain::Brain(): iq = BOSS_IQ" << endl;
//    }
//    // unknown
//    else if (owner->isPet()) {
//        if (((Pet *)owner)->isHellDog()) {
//            cout << "Brain::Brain(): iq = HELLDOG_IQ" << endl;
//            iq = HELLDOG_IQ;
//        }
//    }
//    else {
//        iq = IQ_NONE;
//        cout << "Brain::init(): else { iq = IQ_NONE;" << endl;
//        exit(1);
//    }

    cout << "Boss::init(): hello1" << endl;

    // IQ_NONE : brain이 원래 필요없는 unit.
    // IQ_80   : brain이 원래 필요없는 unit.
    if (iq == IQ_NONE) {
        path[0] = NULL;
        path[1] = NULL;
    }
    else if (iq == IQ_80) {
        pathMaxLen = IQ_80_PATH_MAX_LEN;
        path[0] = new int[pathMaxLen]; // row memory
        path[1] = new int[pathMaxLen]; // ownerCol memory
    }
    else if (iq == IQ_100) {
        pathMaxLen = IQ_100_PATH_MAX_LEN;
        path[0] = new int[pathMaxLen]; // row memory
        path[1] = new int[pathMaxLen]; // ownerCol memory
    }
    else if (iq == IQ_120) {
        pathMaxLen = IQ_120_PATH_MAX_LEN;
        path[0] = new int[pathMaxLen]; // row memory
        path[1] = new int[pathMaxLen]; // ownerCol memory
    }
    else if (iq == IQ_150) {
        pathMaxLen = IQ_150_PATH_MAX_LEN;
        path[0] = new int[pathMaxLen]; // row memory
        path[1] = new int[pathMaxLen]; // ownerCol memory
    }
    
    pathFound = false;
    pathLen = 0;
    curIndexPath = -1;
}

Brain::~Brain() {
    destroy();
}

void Brain::destroy() {
    if (path[0] != NULL) {
        delete [] path[0];
        path[0] = NULL;
    }
    if (path[1] != NULL) {
        delete [] path[1];
        path[1] = NULL;
    }
    // owner는 여기에서 free하지 않는다.
}

void initPath() {
}

void Brain::reset() {
    pathFound = false;
    pathLen = 0;
    curIndexPath = -1;
}

int Brain::findOpenDirVert() {
    int ownerRow = owner->getRow();
    int ownerCol = owner->getCol();
    
    // ver3: random으로 시작하는 짧은 버젼.
    int startDir = (rand() % 2 == 0)?-1:1;
    
    for (int i = 0; i < 2; i++) {
        if (!sim->board->isBlocked(ownerRow + startDir, ownerCol)) {
            return (startDir == -1)?DIR_N:DIR_S;
        }
        startDir *= -1;
    }
    
    return DIR_NONE;
}

// ver4: random으로 시작하는 더 짧은 버젼.
int DIR_ORDERS[2] = {1, -1};
int DIR_HORZS[2] = {DIR_E, DIR_W};

int Brain::findOpenDirHorz() {
    int ownerRow = owner->getRow();
    int ownerCol = owner->getCol();
    
    // ver4: random으로 시작하는 더 짧은 버젼.
    int startIndex = rand() % 2;
    
    for (int i = 0; i < 2; i++)
        if (!sim->board->isBlocked(ownerRow + DIR_ORDERS[(i+startIndex) % 2], ownerCol))
            return DIR_HORZS[(i+startIndex) % 2];
    
    return DIR_NONE;
}

int DIRS_VALS[4] = {-1, 1, 1, -1};

// pre-condition: dir is not DIR_NONE
int Brain::findOpenDir(int dir) {
    if(iq == 80 || iq == 120) {
        return dir;
    }
    else {
        int ownerRow = owner->getRow();
        int ownerCol = owner->getCol();
        
        if (dir == DIR_W || dir == DIR_E) { // start direction is horz
            if (!sim->board->isBlocked(ownerRow, ownerCol + DIRS_VALS[dir])) {
                return dir;
            }
            else {
                int vertDir = DIR_NONE;
                if ((vertDir = findOpenDirVert()) != DIR_NONE) {
                    return vertDir;
                }
                else {
                    if (!sim->board->isBlocked(ownerRow, ownerCol + DIRS_VALS[(dir+2)%4])) { // 처음 주어진 방향의 반대 방향: (dir+2)%4
                        return (dir+2)%4;
                    }
                    else {
                        return DIR_NONE;
                    }
                }
            }
        }
        else { // start direction is vert
            if (!sim->board->isBlocked(ownerRow + DIRS_VALS[dir], ownerCol)) {
                return dir;
            }
            else {
                int horzDir = DIR_NONE;
                if ((horzDir = findOpenDirHorz()) != DIR_NONE) {
                    return horzDir;
                }
                else {
                    if (!sim->board->isBlocked(ownerRow + DIRS_VALS[(dir+2)%4], ownerCol)) { // 처음 주어진 방향의 반대 방향: (dir+2)%4
                        return (dir+2)%4;
                    }
                    else {
                        return DIR_NONE;
                    }
                }
            }
        }

    }
}

void Brain::setPos(int sourceRow, int sourceCol, int dir, int *targetRow, int *targetCol) {
    *targetRow = sourceRow;
    *targetCol = sourceCol;
    
    if (dir == DIR_N) {
        (*targetRow)--;
    }
    if (dir == DIR_E) {
        (*targetCol)++;
    }
    if (dir == DIR_S) {
        (*targetRow)++;
    }
    if (dir == DIR_W) {
        (*targetCol)--;
    }
}

bool Brain::findPathIQ80And100(Pos &targetPos) {
//    cout << "Brain::findPathIQ80And100(): started" << endl;
    int dir = DIR_NONE;
    
    int ownerRow = owner->getRow();
    int ownerCol = owner->getCol();
    
    int targetRow = targetPos.row;
    int targetCol = targetPos.col;
    
//    pathFound = false;
//    pathLen = 0;
//    curIndexPath = -1;
    
//    cout << "Brain::findPathIQ80And100(): hello1" << endl;

    // horz
    if (targetRow == ownerRow) {
        if (targetCol < ownerCol) { // left?
            //                cout << "Boss::move(): if (heroRow == ownerRow) {" << endl;
            if (targetCol+1 != ownerCol) { // adjacent?
                dir = findOpenDir(DIR_W);
                if (dir != DIR_NONE) {
                    setPos(ownerRow, ownerCol, dir, &path[0][0], &path[1][0]);
                    curIndexPath = 0;
                    pathLen = 1;
                    owner->setDir(dir);
                    pathFound = true;
                    return true;
                }
            }
            
        }
        else { // right?
            if (targetCol-1 != ownerCol) { // adjacent?
                dir = findOpenDir(DIR_E);
                if (dir != DIR_NONE) {
                    setPos(ownerRow, ownerCol, dir, &path[0][0], &path[1][0]);
                    curIndexPath = 0;
                    pathLen = 1;
                    owner->setDir(dir);
                    pathFound = true;
                    return true;
                }
            }
        }
    }
    else if (targetCol == ownerCol) { // vert
        if (targetRow < ownerRow) { // up?
            if (targetRow+1 != ownerRow) { // adjacent?
                dir = findOpenDir(DIR_N);
                if (dir != DIR_NONE) {
                    setPos(ownerRow, ownerCol, dir, &path[0][0], &path[1][0]);
                    curIndexPath = 0;
                    pathLen = 1;
                    owner->setDir(dir);
                    pathFound = true;
                    return true;
                }
            }
        }
        else { // down?
            if (targetRow-1 != ownerRow) { // adjacent?
                dir = findOpenDir(DIR_S);
                if (dir != DIR_NONE) {
//                    cout << "Brain::findPathIQ80And100(): hello2" << endl;
                    setPos(ownerRow, ownerCol, dir, &path[0][0], &path[1][0]);
                    curIndexPath = 0;
                    pathLen = 1;
                    owner->setDir(dir);
                    pathFound = true;
                    return true;
                }
            }
        }
    }
    else {
        if (targetRow < ownerRow && targetCol < ownerCol) { // NW
            int diffRow = abs(targetRow - ownerRow);
            int diffCol = abs(targetCol - ownerCol);
            
            if (diffRow > diffCol) { // to north
//                dir = findOpenDir(DIR_N);
//                if (dir != DIR_NONE) {
                    dir = DIR_N;
                    setPos(ownerRow, ownerCol, dir, &path[0][0], &path[1][0]);
                    curIndexPath = 0;
                    pathLen = 1;
                    owner->setDir(dir);
                    pathFound = true;
                    return true;
//                }
            }
            else {
//                dir = findOpenDir(DIR_W);
//                if (dir != DIR_NONE) {
                    dir = DIR_W;
                    setPos(ownerRow, ownerCol, dir, &path[0][0], &path[1][0]);
                    curIndexPath = 0;
                    pathLen = 1;
                    owner->setDir(dir);
                    pathFound = true;
                    return true;
//                }
            }
        }
        else if (targetRow < ownerRow && targetCol > ownerCol) { // NE
            int diffRow = abs(targetRow - ownerRow);
            int diffCol = abs(targetCol - ownerCol);
            
            if(diffRow > diffCol) {
//                dir = findOpenDir(DIR_N);
//                if (dir != DIR_NONE) {
                    dir = DIR_N;
                    setPos(ownerRow, ownerCol, dir, &path[0][0], &path[1][0]);
                    curIndexPath = 0;
                    pathLen = 1;
                    owner->setDir(dir);
                    pathFound = true;
                    return true;
//                }
            }
            else {
//                dir = findOpenDir(DIR_E);
//                if (dir != DIR_NONE) {
                    dir = DIR_E;
                    setPos(ownerRow, ownerCol, dir, &path[0][0], &path[1][0]);
                    curIndexPath = 0;
                    pathLen = 1;
                    owner->setDir(dir);
                    pathFound = true;
                    return true;
//                }
            }
        }
        else if (targetRow > ownerRow && targetCol > ownerCol) { // SE
            int diffRow = abs(targetRow - ownerRow);
            int diffCol = abs(targetCol - ownerCol);
            
            if(diffRow > diffCol) {
//                dir = findOpenDir(DIR_S);
//                if (dir != DIR_NONE) {
                    dir = DIR_S;
                    setPos(ownerRow, ownerCol, dir, &path[0][0], &path[1][0]);
                    curIndexPath = 0;
                    pathLen = 1;
                    owner->setDir(dir);
                    pathFound = true;
                    return true;
//                }
            }
            else {
//                dir = findOpenDir(DIR_E);
//                if (dir != DIR_NONE) {
                    dir = DIR_E;
                    setPos(ownerRow, ownerCol, dir, &path[0][0], &path[1][0]);
                    curIndexPath = 0;
                    pathLen = 1;
                    owner->setDir(dir);
                    pathFound = true;
                    return true;
//                }
            }
        }
        else { //if (heroRow > ownerRow && heroCol < ownerCol) { // SW
            int diffRow = abs(targetRow - ownerRow);
            int diffCol = abs(targetCol - ownerCol);
            
            if(diffRow > diffCol) {
//                dir = findOpenDir(DIR_S);
//                if (dir != DIR_NONE) {
                    dir = DIR_S;
                    setPos(ownerRow, ownerCol, dir, &path[0][0], &path[1][0]);
                    curIndexPath = 0;
                    pathLen = 1;
                    owner->setDir(dir);
                    pathFound = true;
                    return true;
//                }
            }
            else {
//                dir = findOpenDir(DIR_W);
//                if (dir != DIR_NONE) {
                    dir = DIR_W;
                    setPos(ownerRow, ownerCol, dir, &path[0][0], &path[1][0]);
                    curIndexPath = 0;
                    pathLen = 1;
                    owner->setDir(dir);
                    pathFound = true;
                    return true;
//                }
            }
        }
    }
    return false;
}

bool Brain::findPath(Pos &targetPos) {
    cout << "Brain::findPath(): started" << endl;

    if (owner == NULL) {
        return false;
    }
    
//    cout << "Brain::findPath(): hello1" << endl;
    if (path[0] == NULL) { // must init
//        cout << "Brain::findPath(): hello2" << endl;
        init();
    }
    
    // target이 NULL일수도 있다. 왜냐하면, 그냥 어떤 빈 자리가 target일수도 있으므로..
    Unit *targetUnit = sim->board->getUnit(targetPos.row, targetPos.col);
    
    if (iq == 80 || iq == 100) {
        findPathIQ80And100(targetPos);
    }
    
    return pathFound;
}

bool Brain::isPathFound() {
    return pathFound;
}

Pos *Brain::getNextPos() {
    return new Pos(path[0][curIndexPath], path[1][curIndexPath]);
}

void Brain::setNextPos() {
    curIndexPath++;
    if (curIndexPath == pathLen) {
        reset();
    }
}

int Brain::getIq() {
    return iq;
}

bool Brain::findHero(Pos &pos, int range) {
    bool heroFound = false;
//    Hero *hero = NULL;
    pos.row = -1;
    pos.col = -1;
    
    int ownerRow = owner->getRow();
    int ownerCol = owner->getCol();
    
    for (int i = ownerRow-range; i <= ownerRow+range && !heroFound; i++) {
        for (int j = ownerCol-range; j <= ownerCol+range && !heroFound; j++) {
            if (!(i == ownerRow && j == ownerCol) &&
                !sim->board->isOutOfBoundary(i, j) &&
                sim->board->getUnit(i, j) != NULL &&
                sim->board->getUnit(i, j)->isHero()) {
                heroFound = true;
//                hero = (Hero *)board->getUnit(i, j);
                pos.row = i;
                pos.col = j;
            }
        }
    }
    
    return heroFound;
}

bool Brain::findMons(Pos &pos, int range) {
    bool monsFound = false;
    
    pos.row = -1;
    pos.col = -1;
    
    int ownerRow = owner->getRow();
    int ownerCol = owner->getCol();
    
    for (int i = ownerRow-range; i <= ownerRow+range && !monsFound; i++) {
        for (int j = ownerCol-range; j <= ownerCol+range && !monsFound; j++) {
            if (!(i == ownerRow && j == ownerCol) &&
                !sim->board->isOutOfBoundary(i, j) &&
                sim->board->getUnit(i, j) != NULL &&
                sim->board->getUnit(i, j)->isMonster()) {
                monsFound = true;
                //                hero = (Hero *)board->getUnit(i, j);
                pos.row = i;
                pos.col = j;
            }
        }
    }
    
    return monsFound;
}

// pre-condition: owner != NULL && targetUnit != NULL
bool Brain::doesTargetUnitAdjacent(Unit *targetUnit, int row, int col) {
    if (targetUnit == NULL) {
        cout << "Brain::doesTargetUnitAdjacent(): error - if (targetUnit == NULL) {" << endl;
        exit(1);
    }
    
    bool targetFound = false;
    
    if (sim->board->getUnit(row-1, col) == targetUnit) { // N
        targetFound = true;
    }
    else if (sim->board->getUnit(row, col+1) == targetUnit) { // E
        targetFound = true;
    }
    else if (sim->board->getUnit(row+1, col) == targetUnit) { // S
        targetFound = true;
    }
    else if (sim->board->getUnit(row, col-1) == targetUnit) { // W
        targetFound = true;
    }
    
    return targetFound;
}


void Brain::resetPath() {
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// class BrainIQ120
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

BrainIQ120::BrainIQ120() : Brain() {
    iq = IQ_120;
}

BrainIQ120::BrainIQ120(Unit *owner) : Brain(owner) {
    iq = IQ_120;
}

BrainIQ120::~BrainIQ120() {
}

bool BrainIQ120::findPath(Pos &targetPos) {
    if (owner == NULL) {
        return false;
    }
    
    //    cout << "Brain::findPath(): hello1" << endl;
    if (path[0] == NULL) { // must init
        //        cout << "Brain::findPath(): hello2" << endl;
        init();
    }

    // target이 NULL일수도 있다. 왜냐하면, 그냥 어떤 빈 자리가 target일수도 있으므로..
    Unit *targetUnit = sim->board->getUnit(targetPos.row, targetPos.col);

    findPathIQ80And100(targetPos); // IQ_120은 일단 한번 findPathIQ80And100()를 call하고 시작한다.
    
    //        cout << "Brain::findPath(): hello2" << endl;
    
    int ownerRow = owner->getRow();
    int ownerCol = owner->getCol();
    int ownnerDir = owner->getDir();
    if (ownnerDir != DIR_NONE && sim->board->isBlocked(ownerRow, ownerCol, ownnerDir)) {
        //            cout << "Brain::findPath(): hello3" << endl;
        
        // 일단 모든 path관련 member varaible들을 reset하자.
        pathFound = false;
        pathLen = 0;
        curIndexPath = -1;
        
        //------------------------------------------------------------------
        // Wall Following 앨거리듬:
        //------------------------------------------------------------------
        // 준비과정:
        //     - 현재 진행하려는 방향이 막혔으므로, 그 막힌 지점부터 계속 전진하여 막히지 않은 곳이 나오면 goalRow, goalCol로 지정한다.
        // 실제 앨거리듬:
        // 1. 현재 방향으로 전진하려 해본다.
        //      1-a. 막혔다면, 왼쪽으로 돈다. 그리고 다시 1로 간다.
        //      1-b. 안 막혔다면, 전진한 후,(현재 지점을(curRow와 curCol을) path[0][curIndex]와 path[1][curIndex]에 저장한다.)
        //           1-b-1. 현재 지점이 goalRow, goalCol이면 멈추고, path를 따라갈 수 있도록 member variable을 잘 셋해준다.
        //           1-b-1. 오른쪽으로 돌고 1로 간다.
        //------------------------------------------------------------------
        
        int curIndex = 0;
        int curDir = ownnerDir;
        int curRow = ownerRow;
        int curCol = ownerCol;
        int goalRow = ownerRow;
        int goalCol = ownerCol;
        
        int DIR[2][4] = {
            {-1, 0, 1, 0},
            { 0, 1, 0, -1}
        };
        
        calcGoal(goalRow, goalCol, curDir);
        //            cout << "Brain::findPath(): goalRow = " << goalRow << " | goalCol = " << goalCol << endl;
        
        while (true) {
            //                cout << "Brain::findPath(): hello4: while (true) {" << endl;
            
            //--------------------------------------------------------------
            // 0. 현재 지점의 상하좌우를 살펴서, target unit이 있다면 현재 장소가 goal이다.
            //    단, targetPos에 unit이 있을 경우만 check.
            //--------------------------------------------------------------
            if (targetUnit != NULL) {
                if (doesTargetUnitAdjacent(targetUnit, curRow, curCol)) {
                    if (curIndex > 0) {
                        curIndexPath = 0;
                        pathLen = curIndex;
                        pathFound = true;
                    }
                    break;
                }
            }
            
            //--------------------------------------------------------------
            // 1. 현재 방향으로 전진하려 해본다.
            //--------------------------------------------------------------
            if (sim->board->isBlocked(curRow, curCol, curDir)) {
                //      1-a. 막혔다면, 왼쪽으로 돈다. 그리고 다시 1로 간다.
                curDir = (curDir - 1 + NUM_DIRS) % NUM_DIRS;
            }
            else {
                //      1-b. 안 막혔다면, 전진한 후,(현재 지점을(curRow와 curCol을) path[0][curIndex]와 path[1][curIndex]에 저장한다.)
                curRow += DIR[0][curDir];
                curCol += DIR[1][curDir];
                
                path[0][curIndex] = curRow;
                path[1][curIndex] = curCol;
                
                curIndex++;
                
                if (curIndex >= pathMaxLen-1) {
                    pathFound = false;
                    break;
                }
                
                //           1-b-1. 현재 지점이 goalRow, goalCol이면 멈추고, path를 따라갈 수 있도록 member variable을 잘 셋해준다.
                if (curRow == goalRow && curCol == goalCol) {
                    curIndexPath = 0;
                    pathLen = curIndex;
                    pathFound = true;
                    break;
                }
                //           1-b-1. 오른쪽으로 돌고 1로 간다.
                else {
                    curDir = (curDir + 1) % NUM_DIRS;
                }
            }
        }
    }
    
    return pathFound;
}

void BrainIQ120::calcGoal(int &row, int &col, int dir) {
    if(dir == DIR_N) {
        while(row >= 0) {
            if(!sim->board->isBlocked(--row, col, dir)) {
                row--;
                break;
            }
        }
    }
    else if (dir == DIR_E) {
        while (col < COL_SIZE) {
            if(!sim->board->isBlocked(row, ++col, dir)) {
                col++;
                break;
            }
        }
    }
    else if(dir == DIR_S) {
        while(row < ROW_SIZE) {
            if(!sim->board->isBlocked(++row, col, dir)) {
                row++;
                break;
            }
        }
    }
    else if (dir == DIR_W){
        while (col >= 0) {
            if (!sim->board->isBlocked(row, --col, dir)) {
                col--;
                break;
            }
        }
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// class BrainIQ150
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

BrainIQ150::BrainIQ150() : BrainIQ120() {
    iq = IQ_150;
    openSet = new List<Tile>();
    closedSet = new List<Tile>();
}

BrainIQ150::BrainIQ150(Unit *owner) : BrainIQ120(owner) {
    iq = IQ_150;
    openSet = new List<Tile>();
    closedSet = new List<Tile>();
}

void BrainIQ150::resetPath() {
    delete path[0];
    pathMaxLen = IQ_150_PATH_MAX_LEN;
    path[0] = new int[pathMaxLen]; // row memory
    path[1] = new int[pathMaxLen]; // ownerCol memory
}

BrainIQ150::~BrainIQ150() {
}

// pre-condition: openSet->size() > 0
int BrainIQ150::getMinFScoreTileIndex() {
//    assert(openSet->size() > 0);
    
    if (openSet == NULL) {
        cout << "BrainIQ150::getMinScoreTile() : error - openSet is NULL." << endl;
        exit(1);
    }
    
    int minIndex = 0;
    
    Node<Tile> *minNode = openSet->head;
    Node<Tile> *current = openSet->head;
    
    int curIndex = 0;
//    if(temp == NULL) {
//        cout << "BrainIQ150::getMinFScoreTileIndex() : error - openSet->head is NULL." << endl;
//        exit(1);
//    }

    while (current->next != NULL) {
        if (current->element->f < minNode->element->f) {
            minNode = current;
            minIndex = curIndex;
        }
        
        current = current->next;
        curIndex++;
    }
    
//    cout << toBeReturned << endl;
//    exit(1);
    return minIndex;
}

bool BrainIQ150::findPath(Pos &targetPos) {
    if (owner == NULL) {
        return false;
    }
    
    //    cout << "Brain::findPath(): hello1" << endl;
    if (path[0] == NULL) { // must init
        //        cout << "Brain::findPath(): hello2" << endl;
        init();
    }
    
    // target이 NULL일수도 있다. 왜냐하면, 그냥 어떤 빈 자리가 target일수도 있으므로..
    Unit *targetUnit = sim->board->getUnit(targetPos.row, targetPos.col);
    
//    findPathIQ80And100(targetPos); // IQ_120은 일단 한번 findPathIQ80And100()를 call하고 시작한다.
    // IQ100이하쪽에서 못찾았다면, 아래의 IQ120/150을 가동.
    
//    if (BrainIQ120::findPath(targetPos)) {
//        
//        return true;
//    }
    // IQ120쪽에서 못찾았다면, 아래의 A*를 가동.
    
    
//    if (findPathIQ80And100(targetPos) && owner->getDir() != DIR_NONE &&
//        !(sim->board->isBlocked(owner->getRow(), owner->getCol(), owner->getDir()))) {
//        cout << "findPathIQ80And100() : Succeed " << endl;
////        exit(1);
//        return true;
//    }
    
        //            cout << "Brain::findPath(): hello3" << endl;
        
    int curRow = owner->getRow();
    int curCol = owner->getCol();
//    int curDir = owner->getDir();
    
//    cout <<"ownerrowcol" << endl;
//    cout << owner->getRow() << owner->getCol() << endl;
    // 일단 모든 path관련 member varaible들을 reset하자.
    
//    if (curDir != DIR_NONE && sim->board->isBlocked(curRow, curCol, curDir)) {
    pathFound = false;
    pathLen = 0;
    curIndexPath = -1;
//    int curIndex = 0;
    
    //0.일단 내가 서있는 곳은 먼저 closed 되야하므로, 먼저 closedSet에 넣어준다.
    //    closedSet->insert(sim->board->getTile(curRow, curCol));
    //    sim->board->getTile(curRow, curCol)->closed = true;
    //2. goal설정하기
    Tile *goal = sim->board->getTile(targetPos.row, targetPos.col);
    
    //----------------------------------------------------------------------
    // While loop starts
    //----------------------------------------------------------------------
//    cout << "curDir "<< curDir << endl;
    Tile *start = sim->board->getTile(curRow, curCol);
    openSet->insert(start);
    
    sim->board->getTile(curRow, curCol)->open = true;
    sim->board->getTile(curRow, curCol)->g = 0.0;
    sim->board->getTile(curRow, curCol)->h = sim->board->getTile(curRow, curCol)->heuristicCostEstimate(goal);
    sim->board->getTile(curRow, curCol)->f = sim->board->getTile(curRow, curCol)->g + sim->board->getTile(curRow, curCol)->h;
    
    while (openSet->size() > 0) {
//        exit(1);
        Tile *current = openSet->removeElementAt(getMinFScoreTileIndex());
        
        if (current == NULL) {
            cout << "BrainIQ150::findPath(): if (current == NULL) {" << endl;
            exit(1);
        }
        
//        cout << "current : " << current << " | row = " << current->row << " | col = " << current->col << endl;
        
        if (current == goal) {
            reconstructPath(start, goal);
            break;
        }
        
        closedSet->insert(current);
//        cout << "current : closedSet->insert(current);" << endl;
        current->closed = true;

        for (int i = 0; i < NUM_DIRS; i++) {
            Tile *neighTile = sim->board->getNeighTileByDir(current->row, current->col, i);
            
            if (neighTile == NULL) {
                continue;
            }
//            cout << "neighTile : " << neighTile << " | row = " << neighTile->row << " | col = " << neighTile->col << endl;

            if (neighTile->getProp() != NULL) {
                continue;
            }
            
            if (closedSet->find(neighTile)) {
                continue; // Ignore the neighbor which is already evaluated.
            }
            
            if (!openSet->find(neighTile)) {
                openSet->insert(neighTile);
                neighTile->parent = current;
                neighTile->calcFGH(goal);
                neighTile->open = true;
//                cout << "inserted!!" << endl;
            }
                
            double tentativeGScore = current->g + 1.0f;
            if (tentativeGScore >= neighTile->g) {
                continue;
            }
            
            neighTile->parent = current;
            neighTile->g = tentativeGScore;
            neighTile->f = neighTile->g + neighTile->heuristicCostEstimate(goal);
        }
    }
    
    Node<Tile> *temp = openSet->head;
    
    while (temp != NULL) {
        temp->element->resetAStar();
        temp = temp->next;
    }
    
    temp = closedSet->head;
    
    while (temp != NULL) {
        temp->element->resetAStar();
        temp = temp->next;
    }
    
    delete openSet;
    openSet = new List<Tile>();
    
    delete closedSet;
    closedSet = new List<Tile>();
    
    return pathFound;
}

//function A*(start, goal)
//    // The set of nodes already evaluated
//    closedSet := {}
//
//    // The set of currently discovered nodes that are not evaluated yet.
//    // Initially, only the start node is known.
//    openSet := {start}
//
//    // For each node, which node it can most efficiently be reached from.
//    // If a node can be reached from many nodes, cameFrom will eventually contain the
//    // most efficient previous step.
//    cameFrom := the empty map
//
//    // For each node, the cost of getting from the start node to that node.
//    gScor

//
//    // The cost of going from start to start is zero.
//    gScore[start] := 0
//
//    // For each node, the total cost of getting from the start node to the goal
//    // by passing by that node. That value is partly known, partly heuristic.
//    fScore := map with default value of Infinity
//
//    // For the first node, that value is completely heuristic.
//    fScore[start] := heuristic_cost_estimate(start, goal)
//
//    while openSet is not empty
//        current := the node in openSet having the lowest fScore[] value
//        if current = goal
//            return reconstruct_path(cameFrom, current)
//
//        openSet.Remove(current)
//        closedSet.Add(current)
//
//        for each neighbor of current
//            if neighbor in closedSet
//                continue		// Ignore the neighbor which is already evaluated.
//
//            if neighbor not in openSet	// Discover a new node
//                openSet.Add(neighbor)
//
//            // The distance from start to a neighbor
//            tentative_gScore := gScore[current] + dist_between(current, neighbor)
//            if tentative_gScore >= gScore[neighbor]
//                continue		// This is not a better path.
//
//            // This path is the best until now. Record it!
//            cameFrom[neighbor] := current
//            gScore[neighbor] := tentative_gScore
//            fScore[neighbor] := gScore[neighbor] + heuristic_cost_estimate(neighbor, goal)
//
//    return failure

//function reconstruct_path(cameFrom, current)
//    total_path := [current]
//    while current in cameFrom.Keys:
//        current := cameFrom[current]
//        total_path.append(current)
//    return total_path

void recReconstructPath(Tile *start, Tile *current, vector<Tile *> *v) {
    if (start == current) {
        return;
    }
    
    v->push_back(current);
    recReconstructPath(start, current->parent, v);
}

//function reconstruct_path(cameFrom, current)
void BrainIQ150::reconstructPath(Tile *start, Tile *goal) {
    vector<Tile *> *v = new vector<Tile *>();
    
    if (start != goal->parent) {
        // start와 goal을 제외한 모든 path가 reverse order로 v에 들어가면서 recursive가 path를 역으로 따라간다.
        recReconstructPath(start, goal->parent, v);
        
        curIndexPath = 0;
        pathLen = v->size();
        pathFound = true;
        
        // 따라서 반대순서로, 마지막부터 path에 coord를 옮겨주면 된다.
        for (int i = 0; i < pathLen; i++) {
            path[0][i] = (*v)[pathLen-1-i]->row;
            path[1][i] = (*v)[pathLen-1-i]->col;
        }
//        cout << "BrainIQ150::reconstructPath(): start->row = " << start->row << endl;
//        cout << "BrainIQ150::reconstructPath(): start->col = " << start->col << endl;
//        cout << "BrainIQ150::reconstructPath(): goal->row = " << goal->row << endl;
//        cout << "BrainIQ150::reconstructPath(): goal->col = " << goal->col << endl;
//
//        cout << "BrainIQ150::reconstructPath(): pathLen = " << pathLen << endl;
//        for (int i = 0; i < pathLen; i++) {
//            path[0][i] = (*v)[pathLen-1-i]->row;
//            path[1][i] = (*v)[pathLen-1-i]->col;
//            cout << "BrainIQ150::reconstructPath(): path[0][" << i << "] = " << path[0][i] << endl;
//            cout << "BrainIQ150::reconstructPath(): path[1][" << i << "] = " << path[1][i] << endl;
//        }
//        exit(1);
    }
    else {
        curIndexPath = 0;
        pathFound = false;

    }
//    total_path := [current]
//    while current in cameFrom.Keys:
//        current := cameFrom[current]
//        total_path.append(current)
//    return total_path
    
}

