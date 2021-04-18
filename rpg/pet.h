#ifndef PET_H
#define PET_H

class Unit;
class Monster;

class Pet : public Unit {
protected:
    Unit *master;
//    bool timeToReturn;
    int countToReturn;
    
public:
    Pet();
    Pet(char shape, int row, int col, int hp, int maxHp, int mp, int maxMp, int dir, int atk, int def, int gold, int exp, Unit *master, bool timeToReturn);
    virtual void reset();

    virtual ~Pet();
    
    void setMaster(Unit *master);
    
    bool isPet();
    
    virtual bool isHellDog();
    
    virtual void returnToMaster(Unit *master);
    
    virtual bool inMasterRange();
};

class HellDog : public Pet {
protected:
    int range;
    
    Monster *getMons(int row, int col);
public:
    HellDog();
    HellDog(Unit *master);
    virtual void reset();

    virtual ~HellDog();
    
    bool isHellDog();
    
    //void targetFound();
    
    void returnToMaster(Unit *master);
    
    bool inMasterRange();
    
    virtual void move(int dir);
    virtual void interact(Unit *unit);
    
};

#endif /* pet_hpp */
