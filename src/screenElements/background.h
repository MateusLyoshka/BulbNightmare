#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include <genesis.h>
#include "resources.h"
#include "../utils/globals.h"
#include "../utils/utils.h"

typedef enum
{
    ALERT_1,
    ALERT_2,
    ALERT_3,
    ALERT_4,
    ALERT_5,
    ALERT_6,
    BG_LOGO,
    BG_MENU,
    BG_INSTRUCT,
    BG_PAUSE,
    BG_BOSS,
    BG_MAX
} BackgroundType;

extern u16 black_palette[16];
extern u16 target_palette[16];

extern u8 bg_proceed;

u16 BACKGROUND_init_generalized(BackgroundType type, u8 bg, u8 pal, u8 set_fade, u8 prio, u16 ind);
u16 BACKGROUND_clear(u8 bg);
u16 BACKGROUND_full_clear(u16 ind);

#endif