#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include <genesis.h>
#include "resources.h"
#include "../utils/globals.h"

typedef struct
{
    u16 *glow;
    u8 glow_elems;
    u8 glow_current_elem;
    u16 color_ind;
} ColorParams;

typedef enum
{
    BG_B_SELECT = 0,
    BG_A_SELECT = 1
} BackgroundSelect;

u16 BACKGROUND_init(u16 ind);
u16 BACKGROUND_logo_init(u16 ind);
u16 BACKGROUND_clean(BackgroundSelect bg);

extern const u16 logo_color_glow_0[];
extern const u16 logo_color_glow_1[];

extern u8 bg_color_delay;
extern u8 bg_proceed;

static inline void glow_color(u16 color_index, const u16 *const color_vector, u8 n, u8 *current_elem, u8 fade_type)
{
    static u8 inc = 1;

    // kprintf("%d", *current_elem);
    if (fade_type)
    {
        *current_elem += inc;
    }
    else
    {
        *current_elem -= inc;
    }
    PAL_setColor(color_index, color_vector[*current_elem]);
    // kprintf("%d", *current_elem);
    if (*current_elem == 0 || *current_elem == n - 1)
    {
        bg_proceed = 1;
        return;
    }
}

#endif