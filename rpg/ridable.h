#ifndef RIDABLE_H
#define RIDABLE_H

class Unit;

class Ridable : public Unit {
protected:
    Unit *rider;
    
public:
    Ridable();
    Ridable(char shape, int row, int col, int hp, int maxHp, int mp, int maxMp, int dir, int atk, int def, int gold, int exp);
    Ridable(Ridable &other);
    virtual void reset();

    virtual ~Ridable();
    
    bool isRidable();
    
    virtual bool isCar();
    //virtual bool isHorse();
    
    bool isOccupied();
    Unit *getRider();
    void setRider(Unit *rider);

    bool kickOut();
};

class Car : public Ridable {
protected:
public:
    Car();
    Car(Car &other);
    Car(char shape, int row, int col, int hp, int maxHp, int mp, int maxMp, int dir, int atk, int def, int gold, int exp);
    virtual void reset();

    virtual ~Car();
    
//    Unit *getUnit();
//    void setUnit(Unit *unit);
    
    bool isCar();
    
    void printStat();
    
    virtual void move(int dir);
    virtual void interact(Unit *unit);
};

#endif
