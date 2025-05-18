#ifndef _ENEMMIES
#define _ENEMMIES

#include <genesis.h>
#include "resources.h"
#include "../utils/globals.h"
#include "../gameLevels/gameobject.h"
#include "../gameLevels/level.h"
#include "../gameLevels/player.h"

#define LEVEL_1_ENEMY_1 0
#define LEVEL_1_ENEMY_2 1

#define MAX_ENEMIES 20

typedef struct
{
    GameObject firefly;
    u8 type; // ground:0 flying:1
    u16 last_x;
    u16 last_y;
    u8 travel_min_range;
    u8 travel_max_range;
    u8 spawn_screen;
    u8 on_screen;

} Enemy;

u8 ENEMIES_init(u16 ind, u8 level);
void ENEMIES_update_hub(u8 actual_level_enemies, u8 last_level_enemies);
void ENEMIES_g_enemy_update(GameObject *firefly);
void ENEMIES_f_enemy_update(GameObject *firefly, u8 i);
u8 ENEMY_spawn(u8 index, u8 type, u16 last_x, u16 last_y, u8 min_range, u8 max_range, u16 ind);
extern Enemy enemy_pool[MAX_ENEMIES];

#endif