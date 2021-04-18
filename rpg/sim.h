#ifndef SIM_H
#define SIM_H

class Board;
class Statistics;
class Event;

template <class Event>
class PriorityQueue;

class Sim {
public:
    Board *board;
    int time;
    
    PriorityQueue<Event> *eventQueue;
    Statistics *statistics;
    
    Sim();
    void init();

    virtual ~Sim();
};

#endif
