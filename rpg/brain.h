#ifndef BRAIN_H
#define BRAIN_H

//#include "main.h"

class Unit;
class Pos;
class Tile;

template <class T>
class List;

class Brain {
protected:
    Unit *owner;
    int iq;
    
//    int path[2][MAX_LEN_BOSS_PATH]; // row와 col 좌표를 기록하기 위해 row size가 2개 필요하다.
    int *path[2]; // row, col을 위해 2개의 row는 그대로 두고, col길이는 자유롭게 풀었다. 왜냐하면 IQ나 memory크기에 따라서 달라지므로..
    bool pathFound;
    int pathMaxLen; // memory용량. 클수록 긴거리까지 추적가능.
    int pathLen;
    int curIndexPath;
    
    //--------------------------------------------------------------------------
    // General Helper Function
    //--------------------------------------------------------------------------
    void init();

    //--------------------------------------------------------------------------
    // IQ 80/100
    //--------------------------------------------------------------------------
    bool findPathIQ80And100(Pos &targetPos);
    
    // the three helper functions below are used by iq80/100 algorithm
    int findOpenDirVert();
    int findOpenDirHorz();
    int findOpenDir(int dir);
    
    void setPos(int sourceRow, int sourceCol, int dir, int *targetRow, int *targetCol);
    
    
public:
    Brain();
    Brain(Unit *owner);
    
    virtual ~Brain();
    void destroy();

    void reset();
    virtual void resetPath();


    virtual bool findPath(Pos &targetPos);
    bool isPathFound();
    Pos *getNextPos();
    void setNextPos();
    
    virtual int getIq();
    
    bool findHero(Pos &pos, int range);
    bool findMons(Pos &pos, int range);
//    Pos findNearestMon();
//    Pos findNearestBoss();
    
    bool doesTargetUnitAdjacent(Unit *targetUnit, int row, int col);
};

// Wall Following Algorithm
class BrainIQ120 : public Brain {
protected:
    void calcGoal(int &row, int &col, int dir);
    
public:
    BrainIQ120();
    BrainIQ120(Unit *owner);
    
    virtual ~BrainIQ120();
    
    virtual bool findPath(Pos &targetPos);
};

// A* Algorithm
class BrainIQ150 : public BrainIQ120 {
protected:
    List<Tile> *openSet;
    List<Tile> *closedSet;
    
    int getMinFScoreTileIndex();
    
    void reconstructPath(Tile *start, Tile *goal);

public:
    BrainIQ150();
    BrainIQ150(Unit *owner);

    void resetPath();
    
    virtual ~BrainIQ150();
    
    virtual bool findPath(Pos &targetPos);
};

#endif
