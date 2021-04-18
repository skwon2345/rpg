#ifndef BOARD_H
#define BOARD_H

class Item;
class Prop;
class Unit;
class Tile;
class Portal;
class Shockwave;
class Hero;
class Monster;
class Boss;
class Ridable;
class Car;
class Pet;
class Magic;
class Fireball;
class Merchant;

class Board {
private:
    Tile ***board;
    int rowSize;
    int colSize;
    
    Hero *hero;
    
    Monster **mons;
    int maxNumMons;
    int numMons;

    Boss **bosses;
    int maxNumBosses;
    int numBosses;
    
    Merchant **merchants;
    int maxNumMerchants;
    int numMerchants;
    
    Car **cars;
    int maxNumCars;
    int numCars;

    Portal **portals;
    int maxNumPortals;
    int numPortals;
    
    Pet *pet;
    
//    Fireball **fireballs;
//    int maxNumFireballs;
//    int numFireballs;
    
    //--------------------------------------------------------------------------
    // for shockwave
    //--------------------------------------------------------------------------
    Shockwave **shockwaves;
    int maxNumShockwaves;
    int numShockwaves;
    
public:
    Board();
    void init();
    void init(int rowSize, int colSize, int maxNumMons, int maxNumBosses, int maxNumMerchants, int maxNumPotions, int maxNumTrees, int maxNumPortals, int maxNumDaggers, int maxNumCars, int maxNumFireballs, int maxNumTimeBombs);
    
    virtual ~Board();
    
    void print();
    
    int getRowSize();
    int getColSize();
    
    bool isOutOfBoundary(int row, int col);
    
    Tile *getTile(int row, int col);
    Tile *getNeighTileByDir(int row, int col, int dir);

    Hero *getHero();
    
//    Car *getCars(int index);
    
    bool setUnit(int row, int col, Unit *unit);
    Unit *getUnit(int row, int col);
    
    void setProp(int row, int col, Prop *prop);
    Prop *getProp(int row, int col);
    
    void setItem(int row, int col, Item *item);
    Item *getItem(int row, int col);
    
    void setThrowing(int row, int col, Item *item);
    Item *getThrowing(int row, int col);
    
//    void setMagic(int row, int col, Magic *magic);
//    Magic *getMagic(int row, int col);

    Portal *getRandomPortal();
    
    void moveMons();
    void moveBosses();
//    void deadCars();
    void moveCars();
    void movePet();
    
    bool isBlocked(int row, int col);
    bool isBlocked(int row, int col, int dir);
    
    //--------------------------------------------------------------------------
    // for shockwave
    //--------------------------------------------------------------------------
    void initFx();
    void destroyFx();
        
    void clearFx(int row, int col);
    void setFx(int row, int col, char fx);
    void startWave(Unit *waveUnit, Item *waveItem, Prop *waveProp,
                   int row, int col, int range, int frameInterval, char fxShape, int skillNumber);
    
};

#endif
