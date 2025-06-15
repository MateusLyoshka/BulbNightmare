#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <genesis.h>
#include "resources.h"
#include "../utils/gameobject.h"
#include "../screenElements/level.h"
#include "../enemies/enemies.h"
#include "../screenElements/objects.h"
#include "../screenElements/darkness.h"
#include "../screenElements/darkness.h"
// utils

typedef struct
{
    fix16 initial_x;
    fix16 initial_y;
} PlayerSpawnPoint;

typedef struct
{
    u16 x;
    u16 y;
    u16 tile_x;
    u16 tile_y;
} PlayerCenter;

extern PlayerCenter player_center;

#define PLAYER_MAX_GRAVITY 300

extern GameObject player;
extern f16 player_gravity;
extern f16 player_speed;
extern u8 player_is_alive;
extern u8 player_keys;
extern u8 player_lives;
extern u8 switchs_on;
extern u8 player_can_jump;
extern u8 player_can_walk;
extern PlayerSpawnPoint player_spawn;

u16 PLAYER_init(u16 ind);
void PLAYER_update();
void PLAYER_get_input();
u8 PLAYER_on_ground();
void PLAYER_check_collisions();
void PLAYER_object_collision();
void PLAYER_spike_collision();
void PLAYER_enemy_collision();
void PLAYER_respawn();
void PLAYER_free();
void PLAYER_invert_gravity();

#endif