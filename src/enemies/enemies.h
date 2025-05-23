#ifndef _ENEMIES
#define _ENEMIES

#include <genesis.h>
#include "resources.h"
#include "../utils/globals.h"
#include "../gameLevels/gameobject.h"
#include "../gameLevels/level.h"
#include "../gameLevels/player.h"

#define LEVEL_1_ENEMY_1 0
#define LEVEL_1_ENEMY_2 1
#define LEVEL_2_ENEMY_1 2
#define LEVEL_2_ENEMY_2 3
#define LEVEL_2_ENEMY_3 4
#define LEVEL_3_ENEMY_1 5
#define LEVEL_3_ENEMY_2 6
#define LEVEL_3_ENEMY_3 7
#define LEVEL_3_ENEMY_4 8
#define LEVEL_4_ENEMY_1 9
#define LEVEL_4_ENEMY_2 10
#define LEVEL_4_ENEMY_3 11
#define LEVEL_4_ENEMY_4 12
#define LEVEL_4_ENEMY_5 13

#define MAX_ENEMIES 20

extern u8 ENEMIES_enemies_on_level[];

typedef struct
{
    GameObject firefly;
    u8 type; // ground:0 flying:1
    u16 last_x;
    u16 last_y;
    // u8 travel_min_range;
    // u8 travel_max_range;
    u8 spawn_screen;
    u8 spawn_level;
    u8 on_screen;

} Enemy;

void ENEMIES_init();
void ENEMY_init(u8 index, u8 type, u16 last_x, u16 last_y, u8 screen, u8 level);
u8 ENEMIES_spawn_hub(u8 current_level_enemies, u8 last_level_enemies, u8 ind);
u8 ENEMY_spawn(u8 index, u8 ind);
void ENEMY_unspwan(u8 index);
void ENEMIES_update_hub(u8 current_level_enemies, u8 last_level_enemies);
void ENEMIES_g_enemy_update(GameObject *firefly);
// void ENEMIES_f_enemy_update(GameObject *firefly, u8 i);
extern Enemy enemy_pool[MAX_ENEMIES];

#endif