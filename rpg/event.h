#ifndef EVENT_H
#define EVENT_H

class Monster;
class Potion;
class Boss;
class TimeBomb;

class Event {
public:
    int time;
    
    Event();
    Event(int time);
    
    void print();
    
    virtual void trigger() = 0;
};

class MonsterRespawnEvent : public Event {
private:
    Monster *mon;
public:
    MonsterRespawnEvent();
    MonsterRespawnEvent(int time, Monster *mon);
    
    void trigger();
};

class PotionRespawnEvent : public Event {
private:
    Potion *potion;
public:
    PotionRespawnEvent();
    PotionRespawnEvent(int time, Potion *potion);
    
    void trigger();
};

class BossRushEvent : public Event {
private:
    Boss *boss;
public:
    BossRushEvent();
    BossRushEvent(int time, Boss *boss);
    
    void trigger();
};

class TimeBombExplodeEvent : public Event {
private:
    TimeBomb *timeBomb;
public:
    TimeBombExplodeEvent();
    TimeBombExplodeEvent(int time, TimeBomb *timeBomb);
    
    void trigger();
};

class StatLogEvent : public Event {
private:
    int logInterval;
    string logFilename;
public:
    StatLogEvent();
    StatLogEvent(int time, int logInterval, string logFilename);
    
    void trigger();
};

#endif
