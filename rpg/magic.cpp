#include "magic.h"
#include "main.h"
#include "unit.h"
#include "item.h"
#include "prop.h"
#include "board.h"

//extern Board *board;
#include "sim.h"
extern Sim *sim;

Magic::Magic() {
    shape = 'w';
    skillNumber = -1;
    name = "";
    atk = 0;
    delay = -1;
    range = 0;
    mp = 0;
}
Magic::Magic(char shape, int skillNumber, string name, int atk, int delay, int range, int mp) {
    this->shape = shape;
    this->skillNumber = skillNumber;
    this->name = name;
    this->atk = atk;
    this->delay = delay;
    this->range = range;
    this->mp = mp;
}

Magic::~Magic() {
}

string Magic::getName() {
    return name;
}

int Magic::getSkillNumber() {
    return skillNumber;
}

void Magic::print() {
    cout << shape;
}

Fireball::Fireball() : Magic(MAGIC_FIREBALL_SHAPE, MAGIC_FIREBALL_SKILL_NUMBER, MAGIC_FIREBALL_NAME, MAGIC_FIREBALL_ATK, MAGIC_FIREBALL_DELAY, MAGIC_FIREBALL_RANGE, MAGIC_FIREBALL_MP){
}

Fireball::Fireball(char shape, int skillNumber, string magicName, int atk, int delay, int range, int mp) : Magic(shape, skillNumber, name, atk, delay, range, mp) {
}

Fireball::~Fireball() {
}

void Fireball::print() {
    cout << shape;
}

bool Fireball::castMagic(Unit *unit) {
    if (unit->getMp() < mp) {
        return false;
    }
    
    unit->decMp(mp);
    sim->board->startWave(unit, NULL, NULL,
                     unit->getRow(), unit->getCol(),
                     MAGIC_FIREBALL_RANGE,
                     MAGIC_FIREBALL_DELAY,
                     MAGIC_FIREBALL_SHAPE,
                     MAGIC_FIREBALL_SKILL_NUMBER);
    return true;
}

void Fireball::effect(Unit *unit, Item *item, Prop *prop, Unit *user) {
    if (unit != NULL) {
        if(unit->isMonster()) {
            unit->decHp(atk);
        }
        if (unit->isDead()) {
            user->incGold(unit->getGold());
            user->incExp(unit->getExp());
            unit->decGold(unit->getGold());
            unit->decExp(unit->getExp());
        }
    }
}
