#include "darkness.h"

u8 room_lights[MAP_TOTAL_SCREENS];
u16 dark_ind = TILE_USER_INDEX;

void MASK_scroll_init()
{
    for (u8 i = 0; i < MAP_TOTAL_SCREENS; i++)
    {
        room_lights[i] = 0;
    }
    PAL_setColor(33, RGB24_TO_VDPCOLOR(0));
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
}

void MASK_draw()
{
    dark_ind = TILE_USER_INDEX;
    if (room_lights[LEVEL_current_screen] == 0)
    {
        VDP_drawImageEx(BG_A, &dark_mask, TILE_ATTR_FULL(PAL_BACKGROUND_A, 1, 0, 0, dark_ind), 0, 0, false, DMA);
    }
    else
    {
        BACKGROUND_clear(1);
    }
}
void MASK_scroll_update()
{
    if (room_lights[LEVEL_current_screen] == 1)
    {
        BACKGROUND_clear(1);
        room_lights[LEVEL_current_screen] = 2;
    }
    else if (room_lights[LEVEL_current_level] == 0)
    {
        VDP_setHorizontalScroll(BG_A, player_center.x - 34);
        VDP_setVerticalScroll(BG_A, -player_center.y + 34);
    }
}
