#ifndef MONSTER_H
#define MONSTER_H

class Unit;

class Monster : public Unit {
protected:
public:
    Monster();
    Monster(char shape, int row, int col, int hp, int maxHp, int mp, int maxMp, int dir, int atk, int def, int gold, int exp);
    virtual void reset();

    bool isMonster();
    
    virtual void move(int dir);
    virtual void interact(Unit *unit);
};

class Boss : public Monster {
protected:
    int range;
    bool rush;
    
    Hero *getHero(int row, int col);
    
public:
    Boss();
    virtual void reset();
    
    int getRange();
    
    void setRush(bool rush);
    
    bool isBoss();
    
    virtual void move(int dir);
    virtual void interact(Unit *unit);
};

#endif /* monster_hpp */
