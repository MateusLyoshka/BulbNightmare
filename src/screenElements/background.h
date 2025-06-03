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
    ALERT_1,
    ALERT_2,
    ALERT_3,
    ALERT_4,
    ALERT_5,
    BG_LOGO,
    BG_MENU,
    BG_INSTRUCT,
    BG_MAX
} BackgroundType;

extern u16 black_palette[64];
extern u16 target_palette[64];

extern u8 bg_proceed;

u16 BACKGROUND_init_generalized(BackgroundType type, BackgroundSelect bg, u8 pal, u8 set_fade, u16 ind);
u16 BACKGROUND_clear(BackgroundSelect bg);

#endif