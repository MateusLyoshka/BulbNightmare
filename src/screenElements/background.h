#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include <genesis.h>
#include "resources.h"
#include "../utils/globals.h"
#include "../utils/utils.h"

typedef enum
{
    BG_B_SELECT = 0,
    BG_A_SELECT = 1
} BackgroundSelect;

typedef enum
{
    BG_LOGO,
    BG_MENU,
    BG_MENU_1,
    BG_INSTRUCT,
    BG_MAX
} BackgroundType;

extern u16 black_palette[64];
extern u16 target_palette[64];

extern u8 bg_proceed;

u16 BACKGROUND_init_generalized(BackgroundType type, BackgroundSelect bg, u8 pal, u16 ind);
u16 BACKGROUND_clean(BackgroundSelect bg);

#endif