#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <genesis.h>
#include "gameobject.h"
#include "resources.h"

extern GameObject player;
extern f16 player_gravity;
extern f16 player_speed;

u16 PLAYER_init(u16 ind);

void PLAYER_update();

void PLAYER_get_input_lr();

#endif