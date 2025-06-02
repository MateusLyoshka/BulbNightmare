#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include <genesis.h>
#include "resources.h"
#include "../utils/globals.h"
#include "../player/player.h"

// extern const u16 logo_color_glow_1[];
// extern const u16 logo_color_glow_2[];

// extern u8 color_delay;

// u16 MASK_update(u16 ind);
u16 BACKGROUND_init(u16 ind);
u16 BACKGROUND_logo_init(u16 ind);

extern const u16 logo_color_glow_0[];
extern const u16 logo_color_glow_1[];

extern u16 BACKGROUND_clean();

// extern u8 color_delay;

// static inline void glow_color(u16 color_index, const u16 *const color_vector, u8 n, u8 *current_elem, u8 fade_type)
// {
//     static u8 inc = 1;

//     // kprintf("%d", *current_elem);
//     if (fade_type)
//     {
//         *current_elem += inc;
//     }
//     else
//     {
//         *current_elem -= inc;
//     }
//     PAL_setColor(color_index, color_vector[*current_elem]);
//     // kprintf("%d", *current_elem);
//     if (*current_elem == 0 || *current_elem == n - 1)
//     {
//         proceed = 1;
//         return;
//     }
// }

// static inline void color_effects(ColorParams *color_params, u8 max_elems, u8 fade_type)
// {
//     --color_delay;
//     if (color_delay == 0)
//     {
//         for (u8 i = 0; i < max_elems; i++)
//         {
//             glow_color(color_params[i].color_ind, color_params_vec[i].glow, color_params_vec[i].glow_elems, &color_params_vec[i].glow_current_elem, fade_type);
//         }
//         color_delay = 8;
//     }
// }

#endif