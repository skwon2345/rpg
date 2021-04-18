#ifndef MAIN_H
#define MAIN_H

//#define WIN32

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Flag
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//#define BOSS_IQ_120 // 이것을 켜면 IQ 80과 IQ 120이 합쳐져서 IQ 120 앨거리듬이 돌아가고, IQ 120을 끄면, 맨 아래의 IQ 100이 켜진다.
//#define BOSS_IQ_120_TEST_CASE
//
//#ifdef BOSS_IQ_120
//#define BOSS_IQ_80 // 이것을 끄면 아래의 BOSS_IQ_100이 켜진다. BOSS_IQ_120이상을 할 때는, BOSS_IQ_80을 켜야 함.
//#endif
//
//#ifndef BOSS_IQ_80
//#define BOSS_IQ_100
//#endif

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// General
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#define NUM_DIRS 4

#define DIR_NONE -1

#define DIR_N 0
#define DIR_E 1
#define DIR_S 2
#define DIR_W 3

#define DIR_N_CHAR 'w'
#define DIR_E_CHAR 'd'
#define DIR_S_CHAR 's'
#define DIR_W_CHAR 'a'

#define SHAPE_EMPTY '.'

#ifdef WIN32
#define FRAME_DELAY_IN_MILLISEC 100
#else
#define FRAME_DELAY_IN_MILLISEC 30
#endif
#define FPS (1000/FRAME_DELAY_IN_MILLISEC)

//#define MAIN_GAME_LOOP_DELAY FPS * 0.1

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Huge Map
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#define ROW_SIZE 10
#define COL_SIZE 40
#define NUM_TILES (ROW_SIZE*COL_SIZE)

#define MAX_NUM_MONS 3
#define MAX_NUM_BOSSES 0.1
#define MAX_NUM_MERCHANTS 5
#define MAX_NUM_POTIONS 5
#define MAX_NUM_TREES 20
#define MAX_NUM_PORTALS 0.3
#define MAX_NUM_DAGGERS 5
#define MAX_NUM_CARS 5
#define MAX_NUM_FIREBALLS 5
#define MAX_NUM_TIMEBOMBS 5

//+----------------------------------------+
//|...m....Y...............................|
//|..Y..............Y..Y..b................|
//|..Om...................................Y|
//|..Y..............Y.b....................|
//|....................&.......Y...........|
//|...........Y........H.......b..........b|
//|............O................Y..........|
//|.Y......Y.....Ym.b..............Y.......|
//|.......Y...........Y.................Y..|
//|...Y.........Y.......................Y..|
//+----------------------------------------+

//------------------------------------------------------------------------------

#define HUGE_ROW_SIZE (ROW_SIZE*5)
#define HUGE_COL_SIZE (COL_SIZE*5)
#define HUGE_NUM_TILES (HUGE_ROW_SIZE*HUGE_COL_SIZE)

#define HUGE_MAP_RATIO (HUGE_NUM_TILES/(double)NUM_TILES)

#define HUGE_MAX_NUM_MONS (MAX_NUM_MONS*HUGE_MAP_RATIO)
#define HUGE_MAX_NUM_BOSSES (MAX_NUM_BOSSES*HUGE_MAP_RATIO)
#define HUGE_MAX_NUM_MERCHANTS (MAX_NUM_MERCHANTS*HUGE_MAP_RATIO)
#define HUGE_MAX_NUM_POTIONS (MAX_NUM_POTIONS*HUGE_MAP_RATIO)
#define HUGE_MAX_NUM_TREES (MAX_NUM_TREES*HUGE_MAP_RATIO)
#define HUGE_MAX_NUM_PORTALS (MAX_NUM_PORTALS*HUGE_MAP_RATIO)
#define HUGE_MAX_NUM_DAGGERS (MAX_NUM_DAGGERS*HUGE_MAP_RATIO)
#define HUGE_MAX_NUM_CARS (MAX_NUM_CARS*HUGE_MAP_RATIO)
#define HUGE_MAX_NUM_FIREBALLS (MAX_NUM_FIREBALLS*HUGE_MAP_RATIO)
#define HUGE_MAX_NUM_TIMEBOMBS (MAX_NUM_TIMEBOMBS*HUGE_MAP_RATIO)

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Viewport
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#define VIEWPORT_RANGE_ROW 5
#define VIEWPORT_RANGE_COL 20

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Minimap
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#define MINIMAP_TILE_ROW_SIZE 10
#define MINIMAP_TILE_COL_SIZE 10

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// User Commnads
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#define USER_COMMAND_USE_ITEM 'u'

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Unit
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// class Hero
//------------------------------------------------------------------------------
#define SHAPE_HERO 'H'
#define HERO_START_ROW (HUGE_ROW_SIZE/2)
#define HERO_START_COL (HUGE_COL_SIZE/2)

#define HERO_DEFAULT_MAX_HP 1000000
#define HERO_DEFAULT_MAX_MP 100
#define HERO_DEFAULT_ATK 10
#define HERO_DEFAULT_DEF 10
#define HERO_DEFAULT_GOLD 100000000
#define HERO_DEFAULT_EXP 0

#define HERO_REGENERATE_HP_DELAY ((int)(FPS*2))
#define HERO_REGENERATE_MP_DELAY ((int)(FPS*4))

//------------------------------------------------------------------------------
// class Monster
//------------------------------------------------------------------------------
#define SHAPE_MONSTER 'm'
#define MONSTER_DELAY ((int)(FPS*1))

#define MONSTER_DEFAULT_MAX_HP 30
#define MONSTER_DEFAULT_ATK 10
#define MONSTER_DEFAULT_DEF 5
#define MONSTER_DEFAULT_GOLD 100
#define MONSTER_DEFAULT_EXP 10

#define MONSTER_RESPAWN_INTERVAL_MIN_IN_SEC 5.0
#define MONSTER_RESPAWN_INTERVAL_MAX_IN_SEC 10.0
#define MONSTER_RESPAWN_INTERVAL_MIN (int)(MONSTER_RESPAWN_INTERVAL_MIN_IN_SEC*FPS)
#define MONSTER_RESPAWN_INTERVAL_MAX (int)(MONSTER_RESPAWN_INTERVAL_MAX_IN_SEC*FPS)

//------------------------------------------------------------------------------
// class Boss
//------------------------------------------------------------------------------
#define SHAPE_BOSS '&'
#define BOSS_DELAY ((int)(FPS*0.2))

#define BOSS_DEFAULT_MAX_HP 300
#define BOSS_DEFAULT_ATK 30
#define BOSS_DEFAULT_DEF 20
//#define BOSS_DEFAULT_RANGE 5
#define BOSS_DEFAULT_RANGE 15
#define BOSS_DEFAULT_GOLD 20
#define BOSS_DEFAULT_EXP 20

#define BOSS_RUSH_INTERVAL_IN_SEC 5.0
#define BOSS_RUSH_INTERVAL (int)(BOSS_RUSH_INTERVAL_IN_SEC*FPS)


//------------------------------------------------------------------------------
// class Merchant
//------------------------------------------------------------------------------
#define SHAPE_MERCHANT 'G'
#define MAX_LEN_ITEMS_TO_SELL 20
#define MAX_LEN_ITEMS_BOUGHT 10

//------------------------------------------------------------------------------
// class Ridable
//------------------------------------------------------------------------------
#define RIDABLE_DEFAULT_MAX_HP 0
#define RIDABLE_DEFAULT_ATK 0

//------------------------------------------------------------------------------
// class Car
//------------------------------------------------------------------------------
#define SHAPE_CAR '='
#define CAR_DEFAULT_MAX_HP 30
#define CAR_DEFAULT_ATK 0
#define CAR_OFF_CHAR 'q'

#define CAR_DELAY ((int)(FPS*0.2))

//------------------------------------------------------------------------------
// class HellDog
//------------------------------------------------------------------------------
#define SHAPE_HELLDOG '#'
#define HELLDOG_DEFAULT_MAX_HP 300
#define HELLDOG_DEFAULT_ATK 10
#define HELLDOG_DEFAULT_EXP 0
#define HELLDOG_DEFAULT_RANGE 4
#define HELLDOG_MASTER_RANGE 1

#define HELLDOG_DELAY ((int)(FPS*0.1))
#define HELLDOG_RETURN_DELAY ((int)(FPS*3))
//#define HELLDOG_RETURN_DELAY ((int)(FPS*0.1))

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Prop
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#define SHAPE_TREE 'Y'
#define SHAPE_PORTAL 'O'

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Item
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define SHAPE_TIMEBOMB '*'
#define TIMEBOMB_DEFAULT_ATK 100
#define TIMEBOMB_DEFAULT_RANGE 4

#define TIMEBOMB_NAME "Time_Bomb"
#define TIMEBOMB_PRICE 5
#define TIMEBOMB_DISPOSABLE true
#define TIMEBOMB_REMOVABLE true

#define TIMEBOMB_EXPLODE_INTERVAL_IN_SEC 5.0
#define TIMEBOMB_EXPLODE_INTERVAL (int)(TIMEBOMB_EXPLODE_INTERVAL_IN_SEC*FPS)

#define SHAPE_POTION 'b'
#define POTION_DEFAULT_HP 10

#define POTION_NAME "Potion"
#define POTION_PRICE 5
#define POTION_DISPOSABLE true
#define POTION_REMOVABLE false

#define POTION_RESPAWN_INTERVAL_MIN_IN_SEC 5.0
#define POTION_RESPAWN_INTERVAL_MAX_IN_SEC 10.0
#define POTION_RESPAWN_INTERVAL_MIN (int)(POTION_RESPAWN_INTERVAL_MIN_IN_SEC*FPS)
#define POTION_RESPAWN_INTERVAL_MAX (int)(POTION_RESPAWN_INTERVAL_MAX_IN_SEC*FPS)

#define SHAPE_DAGGER '%'
#define DAGGER_DEFAULT_RANGE 4
#define DAGGER_DEFAULT_ATK 10

#define DAGGER_NAME "Dagger"
#define DAGGER_PRICE 5
// 화살이나 bullet의 경우는 range까지 아무것도 부딪히지 않고 날라갔을때, disposable이 true이므로 지워버리고(delete from heap)
// dagger의 경우, 내가 완전하게 의도적으로 대장간에 가서 녹이지 않는 한, 사라지는 것이 아니므로.. disposable이 false
#define DAGGER_DISPOSABLE false
#define DAGGER_REMOVABLE true
#define DAGGER_DURABILITY 100

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Inventory
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define INVENTORY_MAX_NUM_ITEMS 5

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Magicbook
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define MAGICBOOK_MAX_NUM_MAGICS 5

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// FX related
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#define MAX_NUM_SHOCKWAVES 100

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Hero's Magic related
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define MAGIC_FIREBALL_NAME "Fireball"
#define MAGIC_FIREBALL_RANGE 20
#define MAGIC_FIREBALL_DELAY 1
#define MAGIC_FIREBALL_SHAPE '@'
#define MAGIC_FIREBALL_SKILL_NUMBER 0
#define MAGIC_FIREBALL_ATK 10
#define MAGIC_FIREBALL_MP 20

//------------------------------------------------------------------------------
// Weapon related
//------------------------------------------------------------------------------
#define RANDOM_EQUIPPABLE_ITEM_MAX_ITER 10000

#define ITEM_ID_WEAPON "Weapon"

#define DEFAULT_WEAPON_SHAPE 'W'
#define DEFAULT_WEAPON_NAME "Weapon"
#define DEFAULT_WEAPON_PRICE 0
#define DEFAULT_WEAPON_DURABILITY 10
#define DEFAULT_WEAPON_ATK 10
#define DEFAULT_WEAPON_NUM_HANDS 1

//------------------------------------------------------------------------------
// Armor related
//------------------------------------------------------------------------------
#define ITEM_ID_ARMOR "Armor"

#define DEFAULT_ARMOR_SHAPE 'A'
#define DEFAULT_ARMOR_NAME "Armor"
#define DEFAULT_ARMOR_PRICE 0
#define DEFAULT_ARMOR_DURABILITY 10
#define DEFAULT_ARMOR_DEF 10

#define ARMOR_BODYPART_ID_NONE -1
#define ARMOR_BODYPART_ID_HEAD 0
#define ARMOR_BODYPART_ID_CHEST 1
#define ARMOR_BODYPART_ID_GLOVES 2
#define ARMOR_BODYPART_ID_BACK 3
#define ARMOR_BODYPART_ID_LEGS 4
#define ARMOR_BODYPART_ID_FEET 5
#define WEAPON_BODYPART_ID_LEFT_HAND 6
#define WEAPON_BODYPART_ID_RIGHT_HAND 7

#define ARMOR_HEAD_SHAPE 'A'
#define ARMOR_CHEST_SHAPE 'W'
#define ARMOR_GLOVES_SHAPE 'U'
#define ARMOR_BACK_SHAPE 'Q'
#define ARMOR_LEGS_SHAPE 'E'
#define ARMOR_FEET_SHAPE 'Z'
#define WEAPON_ONE_HAND_SHAPE '!'
#define WEAPON_TWO_HAND_SHAPE '?'

#define WEAPON_ONE_HAND 1
#define WEAPON_TWO_HAND 2


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Brain
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// 안 움직이는 아이들
#define IQ_NONE 0

// wandering
#define IQ_80 80
#define IQ_80_PATH_MAX_LEN 1000

// target방향으로 그때마다 한발씩, 대신 막히면 stuck
#define IQ_100 100
#define IQ_100_PATH_MAX_LEN 1

// Following Wall Alogorithm
#define IQ_120 120
#define IQ_120_PATH_MAX_LEN 1000

// A*
#define IQ_150 150
#define IQ_150_PATH_MAX_LEN 1000

#define HERO_IQ IQ_NONE
#define MONSTER_IQ IQ_NONE
#define BOSS_IQ IQ_120
#define HELLDOG_IQ IQ_120

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// class Statistics
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define STAT_LOG_EVENT_INTERVAL_IN_SEC 3.0
#define STAT_LOG_EVENT_INTERVAL (int)(STAT_LOG_EVENT_INTERVAL_IN_SEC*FPS)

#endif
