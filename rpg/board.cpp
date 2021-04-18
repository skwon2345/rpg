#include <iostream>

using namespace std;

#include "main.h"
#include "item.h"
#include "prop.h"
#include "unit.h"
#include "hero.h"
#include "monster.h"
#include "pet.h"
#include "tile.h"
#include "shockwave.h"
#include "board.h"
#include "ridable.h"
#include "magic.h"
#include "merchant.h"
#include "brain.h"
#include "event.h"
#include "priority_queue.h"
#include "statistics.h"


#include "sim.h"
extern Sim *sim;

//char MAP_01[rowSize][colSize+1] = {
//"........................................",
//"........................................",
//"........................................",
//"........................................",
//"........................................",
//"........................................",
//"........................................",
//"........................................",
//"........................................",
//"........................................",
//};

//char MAP_01[ROW_SIZE][COL_SIZE+1] = {
//    "........................................",
//    "........................................",
//    ".....Y.Y................................",
//    ".....Y.Y................................",
//    ".....Y&Y................................",
//    "......Y.................................",
//    "........................................",
//    "........................................",
//    "......H.................................",
//    "........................................",
//};

//char MAP_01[ROW_SIZE][COL_SIZE+1] = {
//    "........................................",
//    "........................................",
//    ".....Y.Y................................",
//    ".....Y.Y................................",
//    ".....YHY................................",
//    "......Y.................................",
//    "......&.................................",
//    "........................................",
//    "........................................",
//    "........................................",
//};

//char MAP_01[ROW_SIZE][COL_SIZE+1] = {
//    "........................................",
//    "........................................",
//    ".....Y.Y................................",
//    ".....Y.Y................................",
//    ".....YmY...H.....m......................",
//    "......Y.................................",
//    "........................................",
//    ".....*..................................",
//    "........................................",
//    "........................................",
//};

//char MAP_01[ROW_SIZE][COL_SIZE+1] = {
//   //0123456789012345678901234567890123456789
//    "........................................", // 0
//    "..........YYY...........................", // 1
//    ".....Y....Y.Y......b....................", // 2
//    "....=Y.Y..Y.Y...........................", // 3
//    ".....YmY..Y.Y...H....G.....*............", // 4
//    "......Y...Y.Y...........................", // 5
//    "..........YYY...........b...............", // 6
//    "..........&.............................", // 7
//    "........................................", // 8
//    "........................................", // 9
//};

//char MAP_01[ROW_SIZE][COL_SIZE+1] = {
//    "........................................",
//    "......Y.................................",
//    ".....Y.Y................................",
//    ".....Y.Y.H..............................",
//    ".....Y.Y................................",
//    ".....Y.Y................................",
//    ".....Y.Y................................",
//    "....&Y.Y................................",
//    ".....Y.Y................................",
//    "......Y.................................",
//};

// BrainIQ150 success
//char MAP_01[ROW_SIZE][COL_SIZE+1] = {
//    "........................................",
//    "......Y.................................",
//    ".....Y.Y................................",
//    ".....Y.Y...H...&........................",
//    ".....Y.Y................................",
//    ".....Y.Y................................",
//    ".....Y.Y................................",
//    ".....Y.Y................................",
//    ".....Y.Y................................",
//    "......Y.................................",
//};

// BrainIQ150 success
//char MAP_01[ROW_SIZE][COL_SIZE+1] = {
//    "........................................",
//    "......Y.................................",
//    ".....Y.Y................................",
//    ".....Y.Y...H..Y&........................",
//    ".....Y.Y................................",
//    ".....Y.Y................................",
//    ".....Y.Y................................",
//    ".....Y.Y................................",
//    ".....Y.Y................................",
//    "......Y.................................",
//};

// BrainIQ150 fail
//char MAP_01[ROW_SIZE][COL_SIZE+1] = {
//    "........................................",
//    "......Y.................................",
//    ".....Y.Y................................",
//    ".....Y.Y...H..Y&........................",
//    ".....Y.Y......Y.........................",
//    ".....Y.Y................................",
//    ".....Y.Y................................",
//    ".....Y.Y................................",
//    ".....Y.Y................................",
//    "......Y.................................",
//};

//char MAP_01[ROW_SIZE][COL_SIZE+1] = {
//    "........................................",
//    "......Y.................................",
//    ".....Y.Y................................",
//    ".....Y.Y...H..Y.........................",
//    ".....Y.Y...&..Y.........................",
//    ".....Y.Y................................",
//    ".....Y.Y................................",
//    ".....Y.Y................................",
//    ".....Y.Y................................",
//    "......Y.................................",
//};


char MAP_01[ROW_SIZE][COL_SIZE+1] = {
    "........................................",
    "......Y.................................",
    ".....Y.Y......Y.........................",
    ".....Y.Y.&....Y..H......................",
    ".....Y.Y......Y.........................",
    ".....Y.Y................................",
    ".....Y.Y................................",
    ".....Y.Y................................",
    ".....Y.Y................................",
    "......Y.................................",
};

Board::Board() {
}

void Board::init() {
    //    init(ROW_SIZE, COL_SIZE, MAX_NUM_MONS, MAX_NUM_BOSSES, MAX_NUM_POTIONS, MAX_NUM_TREES, MAX_NUM_PORTALS);
    init(HUGE_ROW_SIZE, HUGE_COL_SIZE, HUGE_MAX_NUM_MONS, HUGE_MAX_NUM_BOSSES, HUGE_MAX_NUM_MERCHANTS, HUGE_MAX_NUM_POTIONS, HUGE_MAX_NUM_TREES, HUGE_MAX_NUM_PORTALS, HUGE_MAX_NUM_DAGGERS, HUGE_MAX_NUM_CARS, HUGE_MAX_NUM_FIREBALLS, HUGE_MAX_NUM_TIMEBOMBS);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// init()
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void Board::init(int rowSize, int colSize, int maxNumMons, int maxNumBosses, int maxNumMerchants, int maxNumPotions, int maxNumTrees, int maxNumPortals, int maxNumDaggers, int maxNumCars, int maxNumFireballs, int maxNumTimeBombs) {
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifdef BOSS_IQ_120_TEST_CASE // MAP01로 map을 load하여 만드는 경우.
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
    //---------------------------------------------------------------------
    // init the board
    //---------------------------------------------------------------------
    this->rowSize = ROW_SIZE;
    this->colSize = COL_SIZE;
    
    board = new Tile **[this->rowSize];
    
    for (int i = 0; i < this->rowSize; i++) {
        board[i] = new Tile *[this->colSize];
    }
    
    for (int i = 0; i < this->rowSize; i++) {
        for (int j = 0; j < this->colSize; j++) {
//            board[i][j] = new Tile();
            board[i][j] = new Tile(i, j); // for A*
        }
    }
    
    hero = NULL;
    mons = NULL;
    bosses = NULL;
    merchants = NULL;
    cars = NULL;
    portals = NULL;
    
    maxNumMons = 0;
    maxNumBosses = 0;
    maxNumMerchants = 0;
    maxNumCars = 0;
    maxNumPortals = 0;
    
    for (int i = 0; i < this->rowSize; i++) {
        for (int j = 0; j < this->colSize; j++) {
            if (MAP_01[i][j] == SHAPE_MONSTER) {
                maxNumMons++;
            }
            else if (MAP_01[i][j] == SHAPE_BOSS) {
                maxNumBosses++;
            }
            else if (MAP_01[i][j] == SHAPE_MERCHANT) {
                maxNumMerchants++;
            }
            else if (MAP_01[i][j] == SHAPE_CAR) {
                maxNumCars++;
            }
            else if (MAP_01[i][j] == SHAPE_PORTAL) {
                maxNumPortals++;
            }
        }
    }
    
    this->maxNumMons = maxNumMons;
    if (maxNumMons > 0) {
        mons = new Monster *[maxNumMons];
    }
    else {
        mons = NULL;
    }
    numMons = 0;
    
    this->maxNumBosses = maxNumBosses;
    if (maxNumBosses > 0) {
        bosses = new Boss *[maxNumBosses];
    }
    else {
        bosses = NULL;
    }
    numBosses = 0;

    this->maxNumMerchants = maxNumMerchants;
    if (maxNumMerchants > 0) {
        merchants = new Merchant *[maxNumMerchants];
    }
    else {
        merchants = NULL;
    }
    numMerchants = 0;

    this->maxNumCars = maxNumCars;
    if (maxNumCars > 0) {
        cars = new Car *[maxNumCars];
    }
    else {
        cars = NULL;
    }
    numCars = 0;
    
    this->maxNumPortals = maxNumPortals;
    if (maxNumPortals > 0) {
        portals = new Portal *[maxNumPortals];
    }
    else {
        portals = NULL;
    }
    numPortals = 0;
    
    pet = NULL;
    
    for (int i = 0; i < this->rowSize; i++) {
        for (int j = 0; j < this->colSize; j++) {
            if (MAP_01[i][j] == SHAPE_HERO) {
                hero = new Hero();
                setUnit(i, j, hero);
//                hero->setBoard(this);
            }
            else if (MAP_01[i][j] == SHAPE_MONSTER) {
                mons[numMons] = new Monster();
                setUnit(i, j, mons[numMons]);
//                mons[numMons]->setBoard(this);
                numMons++;
            }
            else if (MAP_01[i][j] == SHAPE_BOSS) {
                bosses[numBosses] = new Boss();
                setUnit(i, j, bosses[numBosses]);
//                bosses[numBosses]->setBoard(this);
//                sim->eventQueue->enqueue(new BossRushEvent(sim->time+BOSS_RUSH_INTERVAL, bosses[numBosses]));
                numBosses++;
            }
            else if (MAP_01[i][j] == SHAPE_MERCHANT) {
                merchants[numMerchants] = new Merchant();
                setUnit(i, j, merchants[numMerchants]);
                numMerchants++;
            }
            else if (MAP_01[i][j] == SHAPE_CAR) {
                cars[numCars] = new Car();
                setUnit(i, j, cars[numCars]);
                numCars++;
            }
            else if (MAP_01[i][j] == SHAPE_HELLDOG) {
                pet = new HellDog();
                setUnit(i, j, pet);
            }
            else if (MAP_01[i][j] == SHAPE_TREE) {
                setProp(i, j, new Tree());
            }
            else if (MAP_01[i][j] == SHAPE_PORTAL) {
                portals[numPortals] = new Portal();
                setProp(i, j, portals[numPortals]);
                numPortals++;
            }
            else if (MAP_01[i][j] == SHAPE_POTION) {
                setItem(i, j, new Potion());
            }
            else if (MAP_01[i][j] == SHAPE_TIMEBOMB) {
                setItem(i, j, new TimeBomb());
            }
        }
    }
    
    if (hero == NULL) {
        cout << "Board::init():BOSS_IQ_120_TEST_CASE: if (hero == NULL) {" << endl;
        exit(1);
    }
    
    if (pet != NULL) {
        hero->setPet(pet);
        pet->setMaster(hero);
    }

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#else // random으로 map을 만드는 경우.
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
    //---------------------------------------------------------------------
    // init the board
    //---------------------------------------------------------------------
    this->rowSize = rowSize;
    this->colSize = colSize;
    
    board = new Tile **[rowSize];
    
    for (int i = 0; i < rowSize; i++) {
        board[i] = new Tile *[colSize];
    }
    
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
//            board[i][j] = new Tile();
            board[i][j] = new Tile(i, j); // for A*
        }
    }
    
    //---------------------------------------------------------------------
    // init the hero
    //---------------------------------------------------------------------
    cout << "Board::init(): creating hero.." << endl;
    hero = new Hero();
    cout << "Board::init(): hero = " << hero << endl;
    setUnit(hero->getRow(), hero->getCol(), hero);
    cout << "hello2" << endl;
    Brain *brain = new Brain((Unit *)hero);
    
    //---------------------------------------------------------------------
    // init the HellDog
    //---------------------------------------------------------------------
    cout << "Board::init(): creating HellDog.." << endl;

    pet = new HellDog(hero);
    hero->setPet(pet);
    setUnit(hero->getRow() - 1, hero->getCol() - 1, pet);

    //---------------------------------------------------------------------
    // init the monsters
    //---------------------------------------------------------------------
    cout << "Board::init(): creating mons.." << endl;
    this->maxNumMons = maxNumMons;
    mons = new Monster *[maxNumMons];
    numMons = 0;

    for (int i = 0; i < maxNumMons; i++) {
        while (true) {
            int randRow = rand() % rowSize;
            int randCol = rand() % colSize;
            
            if (getUnit(randRow, randCol) == NULL &&
                getProp(randRow, randCol) == NULL &&
                getItem(randRow, randCol) == NULL) {
                mons[i] = new Monster();
                setUnit(randRow, randCol, mons[i]);
//                mons[i]->setBoard(this);
                break;
            }
        }
        numMons++;
    }
    
    //---------------------------------------------------------------------
    // init the bosses
    //---------------------------------------------------------------------
    cout << "Board::init(): creating bosses.." << endl;

    this->maxNumBosses = maxNumBosses;
    bosses = new Boss *[maxNumBosses];
    numBosses = 0;
    
    for (int i = 0; i < maxNumBosses; i++) {
        while (true) {
            int randRow = rand() % rowSize;
            int randCol = rand() % colSize;
            
            if (getUnit(randRow, randCol) == NULL &&
                getProp(randRow, randCol) == NULL &&
                getItem(randRow, randCol) == NULL) {
                bosses[i] = new Boss();
                setUnit(randRow, randCol, bosses[i]);
//                bosses[i]->setBoard(this);
//                sim->eventQueue->enqueue(new BossRushEvent(sim->time+BOSS_RUSH, bosses[i]));
                break;
            }
        }
        numBosses++;
    }
    
    //---------------------------------------------------------------------
    // init the merchants
    //---------------------------------------------------------------------
    cout << "Board::init(): creating merchants.." << endl;

    this->maxNumMerchants = maxNumMerchants;
    merchants = new Merchant *[maxNumMerchants];
    
    numMerchants = 0;
    
    for (int i = 0; i < maxNumMerchants; i++) {
        while (true) {
            int randRow = rand() % rowSize;
            int randCol = rand() % colSize;
            
            if (getUnit(randRow, randCol) == NULL &&
                getProp(randRow, randCol) == NULL &&
                getItem(randRow, randCol) == NULL) {
                merchants[i] = new Merchant();
                setUnit(randRow, randCol, merchants[i]);
                break;
            }
        }
        numMerchants++;
    }
    
    //---------------------------------------------------------------------
    // init cars
    //---------------------------------------------------------------------
    cout << "Board::init(): creating cars.." << endl;

    this->maxNumCars = maxNumCars;
    cars = new Car *[maxNumCars];
    
    numCars = 0;
    
    for (int i = 0; i < maxNumCars; i++) {
        while (true) {
            int randRow = rand() % rowSize;
            int randCol = rand() % colSize;
            
            if (getUnit(randRow, randCol) == NULL &&
                getProp(randRow, randCol) == NULL &&
                getItem(randRow, randCol) == NULL) {
                cars[i] = new Car();
                setUnit(randRow, randCol, cars[i]);
                break;
            }
        }
        numCars++;
    }
    
    //---------------------------------------------------------------------
    // init trees
    //---------------------------------------------------------------------
    //    board[0][0]->setProp(new Tree()); // 직접 해당 위치의 Tile에게 명령을 내림.
    //    setProp(0, 0, new Tree()); // validation까지 해주는 내 펑션을 call하는 것이고..
    
    for (int i = 0; i < maxNumTrees; i++) {
        while (true) {
            int randRow = rand() % rowSize;
            int randCol = rand() % colSize;
            
            if (getUnit(randRow, randCol) == NULL &&
                getProp(randRow, randCol) == NULL &&
                getItem(randRow, randCol) == NULL) {
                Tree *newTree = new Tree();
                setProp(randRow, randCol, newTree);
                break;
            }
        }
    }
    
    //---------------------------------------------------------------------
    // init portals
    //---------------------------------------------------------------------
    this->maxNumPortals = maxNumPortals;
    portals = new Portal *[maxNumPortals];
//    portalRows = new int[maxNumPortals];
//    portalCols = new int[maxNumPortals];
    
    numPortals = 0;
    
    for (int i = 0; i < maxNumPortals; i++) {
        while (true) {
//            portalRows[i] = rand() % rowSize;
//            portalCols[i] = rand() % colSize;
            int randRow = rand() % rowSize;
            int randCol = rand() % colSize;
            
            if (getUnit(randRow, randCol) == NULL &&
                getProp(randRow, randCol) == NULL &&
                getItem(randRow, randCol) == NULL) {
                portals[i] = new Portal();
                setProp(randRow, randCol, portals[i]);
                break;
            }
        }
        numPortals++;
    }
    
    //---------------------------------------------------------------------
    // init potions
    //---------------------------------------------------------------------
    for(int i = 0; i < maxNumPotions; i++) {
        while(true) {
            int randRow = rand() % rowSize;
            int randCol = rand() % colSize;
            
            if (getUnit(randRow, randCol) == NULL &&
                getProp(randRow, randCol) == NULL &&
                getItem(randRow, randCol) == NULL) {
                setItem(randRow, randCol, new Potion());
                break;
            }
        }
    }
    
    //---------------------------------------------------------------------
    // init daggers
    //---------------------------------------------------------------------
    for(int i = 0; i < maxNumDaggers; i++) {
        while(true) {
            int randRow = rand() % rowSize;
            int randCol = rand() % colSize;
            
            if (getUnit(randRow, randCol) == NULL &&
                getProp(randRow, randCol) == NULL &&
                getItem(randRow, randCol) == NULL) {
                setItem(randRow, randCol, new Dagger());
                break;
            }
        }
    }
    
    //---------------------------------------------------------------------
    // init fireballs
    //---------------------------------------------------------------------
//    for(int i = 0; i < maxNumFireballs; i++) {
//        while(true) {
//            int randRow = rand() % rowSize;
//            int randCol = rand() % colSize;
//            
//            if (getUnit(randRow, randCol) == NULL &&
//                getProp(randRow, randCol) == NULL &&
//                getItem(randRow, randCol) == NULL) {
//                setMagic(randRow, randCol, new Fireball());
//                break;
//            }
//        }
//    }

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // for shockwave
    //--------------------------------------------------------------------------
    initFx();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// destructor
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Board::~Board() {
    destroyFx();

    if (bosses != NULL) {
        // 모든 unit들은 class Tile의 destructor에서 지움.
//        for (int i = 0; i < numBosses; i++) {
//            delete bosses[i];
//        }
        delete [] bosses;
        bosses = NULL;
        numBosses = 0;
    }
    
    if (mons != NULL) {
        // 모든 unit들은 class Tile의 destructor에서 지움.
//        for (int i = 0; i < numMons; i++) {
//            delete mons[i];
//        }
        delete [] mons;
        mons = NULL;
        numMons = 0;
    }
    
    // 모든 unit들은 class Tile의 destructor에서 지움.
//    if (hero != NULL) {
//        delete hero;
//        hero = NULL;
//    }
    if (cars != NULL) {
        delete [] cars;
        cars = NULL;
        numCars = 0;
    }
    
    if (board != NULL) {
        // 아래처럼 만든 순서와 반대로 지워야 한다.
        for (int i = 0; i < rowSize; i++) {
            for (int j = 0; j < colSize; j++) {
                delete board[i][j];
            }
        }
        
        for (int i = 0; i < rowSize; i++) {
            delete [] board[i];
        }

        delete [] board;
        board = NULL;
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// print()
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Board::print() {
    // process all shockwaves --------------------------------------------------
    for (int i = 0; i < maxNumShockwaves; i++) {
        if (shockwaves[i] != NULL) {
            if (!shockwaves[i]->getWaveActivated()) {
                delete shockwaves[i];
                shockwaves[i] = NULL;
                numShockwaves--;
            }
            else {
                shockwaves[i]->print();
            }
        }
    }
    // process all shockwaves end ----------------------------------------------
    
    cout << "-----------------------------------------------" << endl;
    
    cout << "+";
    for (int j = hero->getCol() - VIEWPORT_RANGE_COL; j <= hero->getCol() + VIEWPORT_RANGE_COL; j++) {
        cout << '-';
    }
    cout << "+" << endl;
    
    int minimapRowSize = rowSize / MINIMAP_TILE_ROW_SIZE;
    int minimapColSize = colSize / MINIMAP_TILE_COL_SIZE;

    int minimapRow = 0;

    for (int i = hero->getRow() - VIEWPORT_RANGE_ROW; i <= hero->getRow() + VIEWPORT_RANGE_ROW; i++) {
        cout << "|";
        
        for (int j = hero->getCol() - VIEWPORT_RANGE_COL; j <= hero->getCol() + VIEWPORT_RANGE_COL; j++) {
            if (isOutOfBoundary(i, j)) {
                cout << "#";
            }
            else {
                board[i][j]->print();
            }
        }
        cout << "|";
        
        //----------------------------------------------------------------------
        // print minimap
        //----------------------------------------------------------------------
        if (minimapRow < minimapRowSize) {
            cout << "   ";
            for (int minimapCol = 0; minimapCol < minimapColSize; minimapCol++) {
                //--------------------------------------------------------------
                // scan the current section
                //--------------------------------------------------------------
                Hero *heroFound = NULL;
                Pet *petFound = NULL;
                Boss *bossFound = NULL;
                Portal *portalFound = NULL;
                for (int ii = minimapRow * MINIMAP_TILE_ROW_SIZE; ii < (minimapRow+1) * MINIMAP_TILE_ROW_SIZE; ii++) {
                    for (int jj = minimapCol * MINIMAP_TILE_COL_SIZE; jj < (minimapCol+1) * MINIMAP_TILE_COL_SIZE; jj++) {
                        if (getUnit(ii, jj) != NULL && getUnit(ii, jj)->isHero()) {
                            heroFound = (Hero *)getUnit(ii, jj);
                        }
                        else if ((getUnit(ii, jj) != NULL && getUnit(ii, jj)->isRidable() &&
                                  ((Ridable *)getUnit(ii, jj))->isCar() && ((Ridable *)getUnit(ii, jj))->getRider() != NULL &&
                                  ((Ridable *)getUnit(ii, jj))->getRider()->isHero())) {
                            heroFound = (Hero *)((Ridable *)getUnit(ii, jj))->getRider();
                        }
                        else if (getUnit(ii, jj) != NULL && getUnit(ii, jj)->isPet()) {
                            petFound = (Pet *)getUnit(ii, jj);
                        }
                        else if (getUnit(ii, jj) != NULL && getUnit(ii, jj)->isBoss()) {
                            bossFound = (Boss *)getUnit(ii, jj);
                        }
                        else if (getProp(ii, jj) != NULL && getProp(ii, jj)->isPortal()) {
                            portalFound = (Portal *)getProp(ii, jj);
                        }
                    }
                }
                
                if (heroFound != NULL) {
                    heroFound->print();
                }
                else if (petFound != NULL) {
                    petFound->print();
                }
                else if (bossFound != NULL) {
                    bossFound->print();
                }
                else if (portalFound != NULL) {
                    portalFound->print();
                }
                else {
                    cout << ".";
                }
            }
            
            minimapRow++;
        }
        
        cout << endl;
    }
    
    cout << "+";
    for (int j = hero->getCol() - VIEWPORT_RANGE_COL; j <= hero->getCol() + VIEWPORT_RANGE_COL; j++) {
        cout << '-';
    }
    cout << "+" << endl;
}

int Board::getRowSize() {
    return rowSize;
}

int Board::getColSize() {
    return colSize;
}

bool Board::isOutOfBoundary(int row, int col) {
    return row < 0 || row >= rowSize || col < 0 || col >= colSize;
}

Tile *Board::getTile(int row, int col) {
    if (isOutOfBoundary(row, col)) {
        return NULL;
    }
    
    return board[row][col];
}

Tile *Board::getNeighTileByDir(int row, int col, int dir) {
    int neighRow = row;
    int neighCol = col;
    if (dir == DIR_N) {
        neighRow--;
    }
    else if (dir == DIR_E) {
        neighCol++;
    }
    else if (dir == DIR_S) {
        neighRow++;
    }
    else if (dir == DIR_W) {
        neighCol--;
    }
    
    return sim->board->getTile(neighRow, neighCol);
}

Hero *Board::getHero() {
    return hero;
}

//Car *Board::getCars(int index) {
//    return cars[index];
//}

bool Board::setUnit(int row, int col, Unit *unit) {
    if (isOutOfBoundary(row,col)) { // validation: boundary check
        return false;
    }
    
    board[row][col]->setUnit(unit);
    
    if (unit != NULL) {
        unit->setRow(row);
        unit->setCol(col);
    }
    return true;
}

Unit *Board::getUnit(int row, int col) {
    if (isOutOfBoundary(row,col)) { // validation: boundary check
        return NULL;
    }
    
    return board[row][col]->getUnit();
}

void Board::setProp(int row, int col, Prop *prop) {
    if (isOutOfBoundary(row,col)) { // validation: boundary check
        return;
    }
    
    board[row][col]->setProp(prop);
    
    if (prop != NULL) {
        prop->setRow(row);
        prop->setCol(col);
    }
}

Prop *Board::getProp(int row, int col) {
    if (isOutOfBoundary(row,col)) { // validation: boundary check
        return NULL;
    }
    
    return board[row][col]->getProp();
}

void Board::setItem(int row, int col, Item *item) {
    if (isOutOfBoundary(row,col)) { // validation: boundary check
        return;
    }
    
    board[row][col]->setItem(item);
    
    if (item != NULL) {
        item->setRow(row);
        item->setCol(col);
    }
}

Item *Board::getItem(int row, int col) {
    if (isOutOfBoundary(row,col)) { // validation: boundary check
        return NULL;
    }
    
    return board[row][col]->getItem();
}

void Board::setThrowing(int row, int col, Item *item) {
    if (isOutOfBoundary(row,col)) { // validation: boundary check
        return;
    }
    
    board[row][col]->setThrowing(item);
    
    if (item != NULL) {
        item->setRow(row);
        item->setCol(col);
    }
}

Item *Board::getThrowing(int row, int col) {
    if (isOutOfBoundary(row,col)) { // validation: boundary check
        return NULL;
    }
    
    return board[row][col]->getThrowing();
}

//void Board::setMagic(int row, int col, Magic *magic) {
//    if (isOutOfBoundary(row,col)) { // validation: boundary check
//        return;
//    }
//    
//    board[row][col]->setMagic(magic);
//}
//
//Magic *Board::getMagic(int row, int col) {
//    if (isOutOfBoundary(row,col)) { // validation: boundary check
//        return NULL;
//    }
//    
//    return board[row][col]->getMagic();
//}

Portal *Board::getRandomPortal() {
    return portals[rand() % maxNumPortals];
}

void Board::moveMons() {
    if (mons == NULL) {
        return;
    }

    for (int i = 0; i < maxNumMons; i++) {
        if (mons[i]->isDead() && mons[i]->getRow() != -1) {
            setUnit(mons[i]->getRow(), mons[i]->getCol(), NULL);
            mons[i]->setRow(-1);
            mons[i]->setCol(-1);

            // uniform distribution
            
            int monRespawnRandomInterval = (rand() % (MONSTER_RESPAWN_INTERVAL_MAX - MONSTER_RESPAWN_INTERVAL_MIN + 1)) + MONSTER_RESPAWN_INTERVAL_MIN;
            sim->statistics->totalTimeConsumedRespawnMons += (monRespawnRandomInterval/FPS);
            sim->statistics->totalDeathNumMons++;
            
            sim->eventQueue->enqueue(new MonsterRespawnEvent(sim->time+monRespawnRandomInterval, mons[i]));
        }
        else {
            int randDir = rand() % NUM_DIRS;
            mons[i]->move(randDir);
        }
    }
}

void Board::moveBosses() {
//    cout << "Board::moveBosses(): started" << endl;

    if (bosses == NULL) {
        return;
    }
//    cout << "Board::moveBosses(): hello1" << endl;
    
    for (int i = 0; i < maxNumBosses; i++) {
        if (bosses[i]->isDead() && bosses[i]->getRow() != -1) {
            setUnit(bosses[i]->getRow(), bosses[i]->getCol(), NULL);
            bosses[i]->setRow(-1);
            bosses[i]->setCol(-1);
        }
        else {
            int randDir = rand() % NUM_DIRS;
            bosses[i]->move(randDir);
//            cout << bosses[i]->getIq();
//            exit(1);
        }
    }
}

void Board::moveCars() {
    if (cars == NULL) {
        return;
    }
    
    for (int i = 0; i < maxNumCars; i++) {
        if (cars[i]->isDead() && cars[i]->getRow() != -1) {
            // 만약 unit이 내리지 못하고 있다면, 일단 맵에서 지우지 않는다.
            if (!cars[i]->isOccupied() || cars[i]->kickOut()) {
                setUnit(cars[i]->getRow(), cars[i]->getCol(), NULL);
                cars[i]->setRow(-1);
                cars[i]->setCol(-1);
            }
        }
        // rider가 있다면, 그 rider의 move()이 car의 move()를 call한다.
        // 하지만 자동으로 그 방향으로 계속 움직이게 하려면, 결국 여기에서 frame마다 car들의 move를 call해줘야 한다.
        else {
            cars[i]->move(DIR_NONE); // car는 driver에 의해서 움직이고, driver를 키보드나 그외 컨트롤러로 움직이면 결국 driver의 move에서 car의 direction을 조절하므로.. 여기서는 dir을 줄 필요가 없다.
        }
    }
}

void Board::movePet() {
    if (pet == NULL) {
        return;
    }
    if (pet->isDead() && pet->getRow() != -1) {
        setUnit(pet->getRow(), pet->getCol(), NULL);
        pet->setRow(-1);
        pet->setCol(-1);
    }
    else {
//        pet->move(hero->getDir());
        int randDir = rand() % NUM_DIRS;
//        if (timeToCheckPet) {
//            pet->returnToMaster(getHero());
////            cout <<"Hello" << endl;
////            exit(1);
//        }
//        else {
        pet->move(randDir);
//        }
        //            cout << bosses[i]->getIq();
        //            exit(1);
    }
    
}

//void Board::deadCars() {
//    if(cars == NULL) {
//        return;
//    }
//    
//    for (int i = 0; i < maxNumCars; i++) {
//        if (cars[i]->isDead() && cars[i]->getRow() != -1) {
//            setUnit(cars[i]->getRow(), cars[i]->getCol(), cars[i]->getUnit());
//            cars[i]->setRow(-1);
//            cars[i]->setCol(-1);
//            cars[i]->setRide(false);
//            cars[i]->getUnit()->setRide(false);
//        }
//    }
//}

bool Board::isBlocked(int row, int col) {
    if (isOutOfBoundary(row, col)) {
        return true;
    }
    else {
        return (getUnit(row, col) != NULL || getProp(row, col) != NULL);
    }
}

bool Board::isBlocked(int row, int col, int dir) {
    int nextRow = row;
    int nextCol = col;
    
    if (dir == DIR_N) {
        nextRow--;
    }
    else if (dir == DIR_E) {
        nextCol++;
    }
    else if (dir == DIR_S) {
        nextRow++;
    }
    else if (dir == DIR_W) {
        nextCol--;
    }
    else {
        return false;
    }
    
    if (isOutOfBoundary(nextRow, nextCol)) {
        return true;
    }
    else {
        return (getUnit(nextRow, nextCol) != NULL || getProp(nextRow, nextCol) != NULL);
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// for shockwave
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Board::initFx() {
    maxNumShockwaves = MAX_NUM_SHOCKWAVES;
    shockwaves = new Shockwave *[maxNumShockwaves];
    numShockwaves = 0;
    
    for (int i = 0; i < maxNumShockwaves; i++) {
        shockwaves[i] = NULL;
    }
}

void Board::destroyFx() {
    if (shockwaves != NULL) {
        for (int i = 0; i < maxNumShockwaves; i++) {
            if (shockwaves[i] != NULL) {
                delete shockwaves[i];
            }
        }
        
        delete [] shockwaves;
        shockwaves = NULL;
        
        maxNumShockwaves = 0;
        numShockwaves = 0;
    }
}

void Board::clearFx(int row, int col) {
    board[row][col]->clearFx();
}

void Board::setFx(int row, int col, char fx) {
    board[row][col]->setFx(fx);
}

void Board::startWave(Unit *waveUnit, Item *waveItem, Prop *waveProp,
                      int row, int col, int range, int frameInterval, char fxShape, int skillNumber) {
    if (numShockwaves >= maxNumShockwaves) {
        cout << "error: can't start new shockwave!";
        return;
    }
    
    for (int i = 0; i < maxNumShockwaves; i++) {
        if (shockwaves[i] == NULL) {
            shockwaves[i] = new Shockwave(waveUnit, waveItem, waveProp,
                                          row, col, range, frameInterval, fxShape);
            shockwaves[i]->start();
            shockwaves[i]->setSkillNumber(skillNumber);
            numShockwaves++;
            break;
        }
    }
}
