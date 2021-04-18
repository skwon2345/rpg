#include <iostream>
#include <fstream>
#include <ctime>
//#include <vector>

#include "main.h"

#ifdef WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#endif

using namespace std;

#include "main.h"
#include "item.h"
#include "unit.h"
#include "hero.h"
#include "prop.h"
#include "tile.h"
#include "board.h"
#include "util.h"
#include "brain.h"
#include "sim.h"
#include "event.h"
#include "priority_queue.h"
#include "statistics.h"

Sim *sim = NULL;

//-----------------------------------------------------------------------------
// main()
//-----------------------------------------------------------------------------
int main() {
    
////    vector<int> v;
////    v.push_back(10);
////    v.push_back(20);
////    v.push_back(30);
////    
////    for (int i = 0; i < v.size(); i++) {
////        cout << v[i] << endl;
////    }
//
////    vector<Tile *> v;
////    Tile *tile1 = new Tile();
////    tile1->setItem(new Potion());
////    Tile *tile2 = new Tile();
////    tile2->setProp(new Tree());
////    Tile *tile3 = new Tile();
////    v.push_back(tile1);
////    v.push_back(tile2);
////    v.push_back(tile3);
////    
////    for (int i = 0; i < v.size(); i++) {
////        v[i]->print();
////        cout << endl;
////    }
//
//    exit(1);
    srand((unsigned)time(NULL));
    
    sim = new Sim();
    sim->init();
    
    // 이렇게 유저 인풋을 받아서 sim->board를 customize할 수 있다.
    //    int rowSize, colSize, maxNumMons, maxNumBosses, maxNumPortals;
    //    rowSize = ROW_SIZE;
    //    colSize = COL_SIZE;
    //    maxNumMons = MAX_NUM_MONS;
    //    maxNumBosses = MAX_NUM_BOSSES;
    //    maxNumPortals = MAX_NUM_PORTALS;
    //
    //    sim->board = new sim->board(rowSize, colSize, maxNumMons, maxNumBosses, maxNumPortals);
    
    // huge map을 적용하게 되면 customize를 하지 않는 것이 좋다.
//    sim->board = new sim->board();
//    sim->board->init();
    
    Hero *hero = sim->board->getHero();

    sim->time = 0;

//    //--------------------------------------------------------------------------
//    // testing Statistics
//    //--------------------------------------------------------------------------
//    sim->statistics->printHeader(cout);
//    sim->statistics->printStat(cout);
//    
//    sim->time = 1234;
//    sim->statistics->totalNumStepsHero = 1000;
//    
//    sim->statistics->printStat(cout);
//    
//    exit(1);
//    //--------------------------------------------------------------------------
//
//    //--------------------------------------------------------------------------
//    // testing Statistics: file
//    //--------------------------------------------------------------------------
//    ofstream fout("statistics.txt");
//    sim->statistics->printHeader(fout);
//    sim->statistics->printStat(fout);
//    
//    sim->time = 1234;
//    sim->statistics->totalNumStepsHero = 1000;
//    
//    sim->statistics->printStat(fout);
//    fout.close();
//    
//    exit(1);
//    //--------------------------------------------------------------------------

    sim->eventQueue->enqueue(new StatLogEvent(sim->time+STAT_LOG_EVENT_INTERVAL, STAT_LOG_EVENT_INTERVAL, "statistics.txt"));

    // Main Game Loop
    while (true) { // infinite
        while (sim->eventQueue->size() > 0 &&
               sim->eventQueue->peek()->time <= sim->time) {
            Event *curEvent = sim->eventQueue->dequeue();
            curEvent->trigger();
        }
        
        sim->board->print();
//        cout << "numTicks: " << numTicks << endl;
        hero->printStat();
        cout << endl;
        
        if(hero->isDead()) {
            cout <<"Game Over ! " << endl;
            break;
        }
    
        int heroDir = -1;
        
        if (_kbhit()) {
#ifdef WIN32
            char command = _getch();
#else // MACOS
            char command = getchar();
#endif
            if (command == DIR_N_CHAR || command == toUpperCase(DIR_N_CHAR)) {
                heroDir = DIR_N;
            }
            else if (command == DIR_E_CHAR || command == toUpperCase(DIR_E_CHAR)) {
                heroDir = DIR_E;
            }
            else if (command == DIR_S_CHAR || command == toUpperCase(DIR_S_CHAR)) {
                heroDir = DIR_S;
            }
            else if (command == DIR_W_CHAR || command == toUpperCase(DIR_W_CHAR)) {
                heroDir = DIR_W;
            }
            else {
                heroDir = DIR_NONE;
            }
            
            if (heroDir != DIR_NONE) {
                hero->move(heroDir);
//                cout << hero->getRow() << hero->getBrain()->getRow() << hero->getCol() << hero->getBrain()->getCol();
//                exit(1);
            }
            
            //------------------------------------------------------------------
            // USER_COMMAND_USE_ITEM handler
            //------------------------------------------------------------------
            //            if (command == USER_COMMAND_USE_ITEM || command == toUpperCase(USER_COMMAND_USE_ITEM)) {
            //                hero->useItem();
            //            }
            if (command >= '1' && command <= '5') {
                hero->useItem(command-'1'); // '1'-'5'를 0-4라는 inventory의 index로 바꿔서 call한다.
            }
            //------------------------------------------------------------------
            // Get Off handler
            //------------------------------------------------------------------
            else if (command == 'q' || command == 'Q') {
                hero->getOff();
            }
            //------------------------------------------------------------------
            // Magic: Fireball handler
            //------------------------------------------------------------------
            else if ((command >= '6' && command <= '9') || command == '0') {
                if (command == '0') {
                    hero->effectMagic(command-'0'+ '4' - '0');
                }
                else {
                    hero->effectMagic(command-'6');
                }
            }
            //------------------------------------------------------------------
            // Testing Shockwave handler
            //------------------------------------------------------------------
            else if (command == 'z' || command == 'Z') {
                cout << "main(): calling sim->board->startWave()" << endl;
                sim->board->startWave(NULL, NULL, NULL,
                                 hero->getRow(), hero->getCol(),
                                 5,
                                 1,
                                 '@',
                                 -1);
            }
            //------------------------------------------------------------------
            // Unequip handler
            //------------------------------------------------------------------
            else if (command == 'u' || command == 'U') {
                int bodyPartIDToUnequip = -1;
                while (true) {
                    cout << "Enter the body part ID(0 to 7, or -1 to exit): ";
                    cin >> bodyPartIDToUnequip;
                    if (bodyPartIDToUnequip >= 0 && bodyPartIDToUnequip <= 7) {
                        hero->unequip(bodyPartIDToUnequip);
                        break;
                    }
                    else if (bodyPartIDToUnequip == -1) {
                        break;
                    }
                }
            }
            //------------------------------------------------------------------
            // Backpack handler
            //------------------------------------------------------------------
            else if (command == 'b' || command == 'B') {
                hero->openBackpack();
            }
            //------------------------------------------------------------------
            // Log handler
            //------------------------------------------------------------------
            else if (command == 'l' || command == 'L') {
                sim->statistics->toggleLog();
            }
        }
        
        
        if (sim->time % MONSTER_DELAY == 0) {
            sim->board->moveMons();
        }
        
        if (sim->time % BOSS_DELAY == 0) {
//            cout << "main(): if (numTicks % BOSS_DELAY == 0) {" << endl;
            sim->board->moveBosses();
        }
        
        if (sim->time % CAR_DELAY == 0) {
            sim->board->moveCars();
        }
        
        if (sim->time % HELLDOG_DELAY == 0) {
            sim->board->movePet();
        }
        
        if (sim->time % HERO_REGENERATE_HP_DELAY == 0) {
            hero->incHp(1);
        }

        if (sim->time % HERO_REGENERATE_MP_DELAY == 0) {
            hero->incMp(1);
        }
        
        sim->time++;
        
#ifdef WIN32
        Sleep(FRAME_DELAY_IN_MILLISEC); // millisec 1/1000
        system("cls");
#else
        usleep(FRAME_DELAY_IN_MILLISEC*1000); // microsec 1/1000000
#endif
    }
    
#ifdef WIN32
    getchar();
    getchar();
#endif
    
    delete sim;
    
    return 0;
}
