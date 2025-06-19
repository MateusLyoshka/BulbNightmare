#ifndef _ENEMIES
#define _ENEMIES

#include <genesis.h>
#include "resources.h"
#include "../utils/globals.h"
#include "../utils/gameobject.h"
#include "../screenElements/level.h"
#include "../player/player.h"

#define GROUND 0
#define FLYING 1

#define MAX_ENEMIES 30

extern u8 enemy_counter;

typedef struct
{
    GameObject firefly;
    u8 type; // ground:0 flying:1
    u16 start_x;
    u16 start_y;
    // u8 travel_min_range;
    // u8 travel_max_range;
    u8 spawn_screen;
    u8 spawn_level;
    u8 on_screen;

} Enemy;

void ENEMY_params();
void ENEMY_init(u8 index, u8 type, u16 last_x, u16 last_y, u8 screen, u8 level);
u8 ENEMIES_spawn_hub(u8 ind);
u8 ENEMY_spawn(u8 index, u8 ind);
void ENEMY_unspwan(u8 index);
void ENEMIES_update_hub();
void ENEMIES_g_enemy_update(GameObject *firefly);
void ENEMIES_level_change_despawn();
// void ENEMIES_f_enemy_update(GameObject *firefly, u8 i);
extern Enemy enemy_pool[MAX_ENEMIES];

#endif