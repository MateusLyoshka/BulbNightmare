#include "darkness.h"

u8 room_lights[MAP_TOTAL_SCREENS];

u16 mask_scroll_init(u16 ind)
{
    for (u8 i = 0; i < MAP_TOTAL_SCREENS; i++)
    {
        room_lights[i] = 0;
    }
    PAL_setColor(33, RGB24_TO_VDPCOLOR(0));
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
    VDP_drawImageEx(BG_A, &dark_mask, TILE_ATTR_FULL(PAL_BACKGROUND_A, 1, 0, 0, ind), 0, 0, false, DMA);
    ind += dark_mask.tileset->numTile;
    return ind;
}

void mask_scroll_update()
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
