#ifndef _HUD_H
#define _HUD_H

#include <genesis.h>
#include "resources.h"
#include "../utils/gameobject.h"
#include "../utils/globals.h"
#include "../player/player.h"

extern u8 player_gems;

u16 HUD_init(u16 ind);
void HUD_update();
void HUD_clear();

#endif