#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <genesis.h>
#include "gameobject.h"
#include "resources.h"
#include "level.h"
#include "../enemies/enemies.h"
#include "objects.h"
// utils

#define PLAYER_MAX_GRAVITY 300

extern GameObject player;
extern f16 player_gravity;
extern f16 player_speed;
extern u8 player_is_alive;
extern u8 player_have_key;

u16 PLAYER_init(u16 ind);
void PLAYER_update();
void PLAYER_get_input_lr();
u8 PLAYER_on_ground();
void PLAYER_check_death();

#endif