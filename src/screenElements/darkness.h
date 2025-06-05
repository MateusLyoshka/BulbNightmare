#ifndef _DARKNESS
#define _DARKNESS

#include <genesis.h>
#include "background.h"
#include "../player/player.h"

extern u8 room_lights[MAP_TOTAL_SCREENS];
extern u16 dark_ind;

void MASK_scroll_init();
void MASK_draw();
void MASK_scroll_update();

#endif