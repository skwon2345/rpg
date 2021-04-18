#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

#include "main.h"
#include "statistics.h"

#include "sim.h"
extern Sim *sim;

Statistics::Statistics() {
    totalNumStepsHero = 0;
    totalMoneySpentHero = 0;
    
    avgTimeConsumedRespawnMons = 0;
    totalDeathNumMons = 0;
    totalTimeConsumedRespawnMons = 0;
    totalNumRespawnMons = 0;
    
    logging = true;
}

Statistics::~Statistics() {}

//Time      |TtlSteps  |
//----------+----------+
//         0|         0|
void Statistics::printHeader(ostream &out) {
    if (logging) {
        out << setw(10) << "Time" << "|" << setw(15) << "Ttl_Steps" << "|" << setw(35) << "Avg_Time_Consumed_Respawn_Mons" << "|" << setw(24) << "Ttl_Money_Spent_Hero" << "|" << endl;
        out << "----------+---------------+-----------------------------------+------------------------+" << endl;
    }
}

void Statistics::printStat(ostream &out) {
    if (logging) {
        out << setw(10) << right << fixed << setprecision(1) << (sim->time / FPS) << "|" << setw(15) << right << totalNumStepsHero << "|" << setw(35) << right << avgTimeConsumedRespawnMons << "|" << setw(24) << right << totalMoneySpentHero << "|" << endl;
    }
}

void Statistics::toggleLog() {
    logging = !logging;
}

void Statistics::turnOnLog() {
    logging = true;
}

void Statistics::turnOffLog() {
    logging = false;
}
