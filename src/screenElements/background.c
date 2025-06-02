#include "background.h"

u8 bg_color_delay = 5;
u8 bg_proceed = 0;

u16 BACKGROUND_init(u16 ind)
{

    PAL_setPalette(PAL_BACKGROUND_B, menu_bg.palette->data, DMA);

    VDP_drawImageEx(BG_B, &menu_bg, TILE_ATTR_FULL(PAL_BACKGROUND_B, 0, 0, 0, ind), 0, 0, FALSE, TRUE);

    return ind + 1;
}

u16 BACKGROUND_logo_init(u16 ind)
{
    PAL_setPalette(PAL_BACKGROUND_B, utf_logo.palette->data, DMA);

    VDP_drawImageEx(BG_B, &utf_logo,
                    TILE_ATTR_FULL(PAL_BACKGROUND_B, 0, 0, 0, ind),
                    0, 0, FALSE, TRUE);

    return ind;
}

u16 BACKGROUND_clean(BackgroundSelect bg)
{
    VDP_clearPlane(bg == BG_B_SELECT ? BG_B : BG_A, TRUE);

    UTILS_clear_palette(bg == BG_B_SELECT ? PAL_BACKGROUND_B : PAL_BACKGROUND_B);

    return TILE_USER_INDEX;
}
