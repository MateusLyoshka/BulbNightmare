#ifndef _HUD_H
#define _HUD_H

#include <genesis.h>
#include "../utils/globals.h"
#include "gameobject.h"
#include "resources.h"

extern u8 player_gems;

u16 HUD_init(u16 ind);
void HUD_update(s16 light_power, s16 keys);

#endif