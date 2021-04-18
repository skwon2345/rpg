#ifndef SHOCKWAVE_H
#define SHOCKWAVE_H

class Unit;
class Item;
class Prop;
class Board;

class Shockwave {
private:
    bool waveActivated;
    
    int waveStartRow;
    int waveStartCol;
    int waveMaxRange;
    int waveCurPhase;
    int waveFrameInterval; // to skip frames
    int waveCountFrames;
    int waveFxShape;

    Unit *waveUnit;
    Item *waveItem;
    Prop *waveProp;

    bool drawn;
    
    void removePrevious();
    void drawCurrentPhase();
public:
    
    int skillNumber;
    
    Shockwave();
    Shockwave(Unit *waveUnit, Item *waveItem, Prop *waveProp,
              int row, int col, int range, int frameInterval, char fxShape);

    virtual ~Shockwave();
    
    void start();
    void stop();
    void print();
    
    int getSkillNumber();
    void setSkillNumber(int skillNumber);
    
    bool getWaveActivated();
};

#endif
