#ifndef STATISTICS_H
#define STATISTICS_H

class Statistics {
public:
    bool logging;

    int totalNumStepsHero;
//    char moneySpentFrom;
//    int moneySpentHero;
    int totalMoneySpentHero;
    
    int avgTimeConsumedRespawnMons;
    int totalDeathNumMons;
    int totalTimeConsumedRespawnMons;
    int totalNumRespawnMons;
    
    Statistics();
    virtual ~Statistics();
    
    void printHeader(ostream &out);
    void printStat(ostream &out);
    
    void toggleLog();
    void turnOnLog();
    void turnOffLog();
};

#endif
