#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include <genesis.h>
#include "resources.h"

typedef enum
{
    BG_TITLE,
    BG_GAME,
    BG_WHITE,
    // BG_OPTIONS,
    // BG_CREDITS,
    BG_COUNT
} BackgroundType;

u16 BACKGROUND_show(BackgroundType type, u16 tile_index);

#endif