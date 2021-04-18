#include <iostream>

using namespace std;

#include "unit.h"
#include "item.h"
#include "prop.h"
#include "board.h"
#include "shockwave.h"
#include "hero.h"

//extern Board *board;
#include "sim.h"
extern Sim *sim;

Shockwave::Shockwave() {
    waveActivated = false;
    waveStartRow = -1;
    waveStartCol = -1;
    waveMaxRange = 0;
    waveFrameInterval = 0;
    waveCurPhase = 0;
    waveCountFrames = 0;
    waveFxShape = ' ';
    
    skillNumber = 0;
}

Shockwave::Shockwave(Unit *waveUnit, Item *waveItem, Prop *waveProp,
                     int row, int col, int range, int frameInterval, char fxShape) {
    waveActivated = false;
    waveStartRow = row;
    waveStartCol = col;
    waveMaxRange = range;
    waveCurPhase = 1; // don't begin from the start position.
    waveFrameInterval = frameInterval;
    waveCountFrames = 0;
    waveFxShape = fxShape;
    drawn = false;
    
    this->waveUnit = waveUnit;
    this->waveItem = waveItem;
    this->waveProp = waveProp;

    skillNumber = 0;
}

Shockwave::~Shockwave() {
}

void Shockwave::start() {
    waveActivated = true;
}

void Shockwave::stop() {
    waveActivated = false;
}

int Shockwave::getSkillNumber() {
    return skillNumber;
}

void Shockwave::setSkillNumber(int skillNumber) {
    this->skillNumber = skillNumber;
}

void Shockwave::removePrevious() {
    if (waveCurPhase > 2 || drawn) {
        int curRange = waveCurPhase - 1;
        for (int i = waveStartRow-curRange; i <= waveStartRow+curRange; i++) {
            for (int j = waveStartCol-curRange; j <= waveStartCol+curRange; j++) {
                // if the current tile is on the boundary frame.
                if (!sim->board->isOutOfBoundary(i, j) && (
                                              i == waveStartRow-curRange || i == waveStartRow+curRange ||
                                              j == waveStartCol-curRange || j == waveStartCol+curRange)) {
                    // remove the previous fx from the current tile.
                    sim->board->clearFx(i, j);
                }
            }
        }
        
        waveCurPhase++;
        
        if (waveCurPhase == waveMaxRange+1) {
            waveActivated = false;
        }
    }
}

void Shockwave::drawCurrentPhase() {
    // draw the current fx
    int curRange = waveCurPhase - 1;
    for (int i = waveStartRow-curRange; i <= waveStartRow+curRange; i++) {
        for (int j = waveStartCol-curRange; j <= waveStartCol+curRange; j++) {
            // if the current tile is on the boundary frame.
            if (!sim->board->isOutOfBoundary(i, j) && (
                                          i == waveStartRow-curRange || i == waveStartRow+curRange ||
                                          j == waveStartCol-curRange || j == waveStartCol+curRange)) {
                // remove the previous fx from the current tile.
                sim->board->setFx(i, j, waveFxShape);
                
                // call the starter object of this shockwave
                if (waveUnit != NULL) {
                    waveUnit->effect(sim->board->getUnit(i, j), sim->board->getItem(i, j), sim->board->getProp(i, j), skillNumber, waveUnit);
                }
                else if (waveItem != NULL) {
                    waveItem->effect(sim->board->getUnit(i, j), sim->board->getItem(i, j), sim->board->getProp(i, j), sim->board->getHero());
                }
                else if (waveProp != NULL) {
//                    waveProp->effect(board->getUnit(i, j), board->getItem(i, j), board->getProp(i, j));
                }
            }
        }
    }
    
    drawn = true;
    
    waveCountFrames = 0;
}

void Shockwave::print() {
    if (waveActivated) {
        if (waveCurPhase > 1 && waveCurPhase <= waveMaxRange+1) { // started to draw fx square yet
            // remove the previous frame when waveCountFrames reaches waveFrameInterval
            if (waveCountFrames >= waveFrameInterval) {
                removePrevious();
                
                if (waveCurPhase < waveMaxRange+1) {
                    drawCurrentPhase();
                }
                else { // the the starter waveItem of this shockwave should be disposed
                    if (waveItem != NULL) {
//                        waveItem->dispose();
//                        if (waveItem->hasBeenDisposed()) {
//                            if (board->getItem(waveItem->getRow(), waveItem->getCol()) == waveItem) {
//                                board->setItem(waveItem->getRow(), waveItem->getCol(), NULL);
//                            }
//                            
//                            delete waveItem;
//                            waveItem = NULL;
//                        }
                        
                        if (waveItem->isDisposable()) { // time bomb같은 애들 마지막에 지워준다.
                            if (waveItem->getRow() != -1 &&
                                sim->board->getItem(waveItem->getRow(), waveItem->getCol()) == waveItem) {
                                sim->board->setItem(waveItem->getRow(), waveItem->getCol(), NULL);
                            }
                            delete waveItem;
                            waveItem = NULL;
                        }

                    }
                }
            }
            waveCountFrames++;
        }
        else {
            waveCurPhase++;
        }
    }
}

bool Shockwave::getWaveActivated() {
    return waveActivated;
}
