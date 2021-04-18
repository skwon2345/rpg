#ifndef PROP_H
#define PROP_H

class Unit;
class Board;

class Prop {
protected:
    char shape;
    int row;
    int col;
    
public:
    Prop();
    Prop(char shape);
    
    void print();
    
    int getRow();
    void setRow(int row);
    
    int getCol();
    void setCol(int col);
    
    virtual bool isClimbable();
    
    virtual bool isPortal();
    
    virtual void trigger(Unit *unit) = 0;
};

class Tree : public Prop {
protected:
public:
    Tree();

    virtual void trigger(Unit *unit);
};

class Portal : public Prop {
protected:
public:
    Portal();
    Portal(int row, int col);

    bool isClimbable();

    bool isPortal();

    virtual void trigger(Unit *unit);
};

#endif
