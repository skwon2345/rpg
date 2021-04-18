#ifndef TILE_H
#define TILE_H

#define FX_NONE '\0'
#define THROWING_NONE NULL

class Magic;
class Pos;
class Item;
class Unit;
class Prop;

class Tile {
private:
    char fx; // fx layer
    
//    Magic *magic;
    
    Item *throwing;
    
    Unit *unit;
    Item *item;
    Prop *prop;
    
public:
    //--------------------------------------------------------------------------
    // A*용 변수들과 functions
    //--------------------------------------------------------------------------
    int row; // me
    int col; // me
    
    Tile *parent;
    
    bool open;
    bool closed;
    
    double f;
    double g;
    double h;
    
    void resetAStar();
    double heuristicCostEstimate(Tile *goal);
    void calcFGH(Tile *goal); // me <-> goal

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // Member Fucntions
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    Tile();
    Tile(int row, int col);
    virtual ~Tile();
    
    void print() ;
    
    char getFx();
    void setFx(char fx);
    void clearFx(); // resteFx()
    
//    Magic *getMagic();
//    void setMagic(Magic *magic);

    Item *getThrowing();
    void setThrowing(Item *throwing);
    void clearThrowing();

    Unit *getUnit();
    void setUnit(Unit *unit);
    
    Item *getItem();
    void setItem(Item *item);
    
    Prop *getProp();
    void setProp(Prop *prop);

};

#endif
