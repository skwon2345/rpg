#include <iostream>
#include <fstream>

using namespace std;

#include "main.h"
#include "unit.h"
#include "monster.h"
#include "event.h"
#include "board.h"
#include "item.h"
#include "statistics.h"
#include "priority_queue.h"

#include "sim.h"
extern Sim *sim;

//------------------------------------------------------------------------------
// class Event
//------------------------------------------------------------------------------
Event::Event() {
    time = 0;
}

Event::Event(int time) {
    this->time = time;
}

void Event::print() {
    cout << "time: " << time;
}

//------------------------------------------------------------------------------
// class MonsterRespawnEvent
//------------------------------------------------------------------------------
MonsterRespawnEvent::MonsterRespawnEvent() : Event() {
    mon = NULL;
}

MonsterRespawnEvent::MonsterRespawnEvent(int time, Monster *mon) : Event(time) {
    this->mon = mon;
}

void MonsterRespawnEvent::trigger() {
    while (true) {
        int randRow = rand() % sim->board->getRowSize();
        int randCol = rand() % sim->board->getColSize();
        
        if (sim->board->getUnit(randRow, randCol) == NULL &&
            sim->board->getProp(randRow, randCol) == NULL &&
            sim->board->getItem(randRow, randCol) == NULL) {
            mon->reset();
            
            sim->statistics->totalNumRespawnMons++;
            //death는 많은데 아직 respawn이 안되었을 수도 있기 때문에
//            if (sim->statistics->totalNumRespawnMons == sim->statistics->totalDeathNumMons) {
            sim->statistics->avgTimeConsumedRespawnMons = ((sim->statistics->totalDeathNumMons == 0) ? 0 : (sim->statistics->totalTimeConsumedRespawnMons/sim->statistics->totalDeathNumMons));
//            }
            
            sim->board->setUnit(randRow, randCol, mon);
            break;
        }
    }
    delete this;
}

//------------------------------------------------------------------------------
// class PotionRespawnEvent
//------------------------------------------------------------------------------
PotionRespawnEvent::PotionRespawnEvent() : Event() {
    potion = NULL;
}

PotionRespawnEvent::PotionRespawnEvent(int time, Potion *potion) : Event(time) {
    this->potion = potion;
}

void PotionRespawnEvent::trigger() {
    while (true) {
        int randRow = rand() % sim->board->getRowSize();
        int randCol = rand() % sim->board->getColSize();
        
        if (sim->board->getUnit(randRow, randCol) == NULL &&
            sim->board->getProp(randRow, randCol) == NULL &&
            sim->board->getItem(randRow, randCol) == NULL) {
//            potion->reset();
            sim->board->setItem(randRow, randCol, potion);
            break;
        }
    }
    delete this;
}

//------------------------------------------------------------------------------
// class BossRushEvent
//------------------------------------------------------------------------------
BossRushEvent::BossRushEvent() : Event() {
    boss = NULL;
}
BossRushEvent::BossRushEvent(int time, Boss *boss) : Event(time) {
    this->boss = boss;
}

void BossRushEvent::trigger() {
    boss->setRush(true);
    delete this;
}

//------------------------------------------------------------------------------
// class TimeBombExplodeEvent
//------------------------------------------------------------------------------
TimeBombExplodeEvent::TimeBombExplodeEvent() : Event() {
    timeBomb = NULL;
}

TimeBombExplodeEvent::TimeBombExplodeEvent(int time, TimeBomb *timeBomb) : Event(time) {
    this->timeBomb = timeBomb;
}

void TimeBombExplodeEvent::trigger() {
    timeBomb->explode();
    delete this;
}

//------------------------------------------------------------------------------
// class StatLogEvent
//------------------------------------------------------------------------------
//int logInterval;
StatLogEvent::StatLogEvent() : Event() {
    logInterval = 0;
    logFilename = "";
}

StatLogEvent::StatLogEvent(int time, int logInterval, string logFilename) : Event(time) {
    this->logInterval = logInterval;
    this->logFilename = logFilename;
    
    ofstream logFile;
    logFile.open(logFilename.c_str(), ofstream::out);
    sim->statistics->printHeader(logFile);
    logFile.close();
}

void StatLogEvent::trigger() {
    ofstream logFile;
    logFile.open(logFilename.c_str(), ofstream::out | ofstream::app);
    sim->statistics->printStat(logFile);
    logFile.close();
    
    // re-scheduling
    time = sim->time + logInterval;
    sim->eventQueue->enqueue(this);
}
