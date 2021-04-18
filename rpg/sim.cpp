#include <iostream>

using namespace std;

#include "main.h"
#include "board.h"
#include "sim.h"
#include "priority_queue.h"
#include "statistics.h"
#include "event.h"

Sim::Sim() {
    board = new Board();
    time = 0;
    
    eventQueue = new PriorityQueue<Event>();
    statistics = new Statistics();
}

void Sim::init() {
    board->init();
}

Sim::~Sim() {
    if (statistics != NULL) {
        delete statistics;
        statistics = NULL;
    }

    if (eventQueue != NULL) {
        delete eventQueue;
        eventQueue = NULL;
    }

    if (board != NULL) {
        delete board;
        board = NULL;
    }
}
