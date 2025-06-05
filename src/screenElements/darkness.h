#ifndef _DARKNESS
#define _DARKNESS

#include <genesis.h>
#include "background.h"
#include "../player/player.h"

extern u8 room_lights[MAP_TOTAL_SCREENS];

u16 mask_scroll_init(u16 ind);
void mask_scroll_update();

#endif