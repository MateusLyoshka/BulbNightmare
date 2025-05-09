#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <genesis.h>
#include "gameobject.h"
#include "resources.h"

#define PLAYER_SPEED FIX16(2)

extern GameObject player;

u16 PLAYER_init(u16 ind);

void PLAYER_update();

void PLAYER_get_input_dir2();

#endif