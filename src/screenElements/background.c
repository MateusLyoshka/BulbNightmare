#include "background.h"

const Image *background_images[BG_MAX] = {
    [ALERT_1] = &alert_1,
    [ALERT_2] = &alert_2,
    [ALERT_3] = &alert_3,
    [ALERT_4] = &alert_4,
    [ALERT_5] = &alert_5,
    [BG_LOGO] = &utf_logo,
    [BG_MENU] = &menu_bg,
    [BG_INSTRUCT] = &instructions,
};
u8 bg_proceed = 0;

u16 black_palette[64] = {0};
u16 target_palette[64];

u16 BACKGROUND_init_generalized(BackgroundType type, BackgroundSelect bg, u8 pal, u8 set_fade, u16 ind)
{
    ind = BACKGROUND_full_clear(ind);
    if (type >= BG_MAX || background_images[type] == NULL)
        return ind;

    const Image *img = background_images[type];
    PAL_setPalette(pal, img->palette->data, DMA);
    if (set_fade)
    {
        PAL_getPalette(pal, target_palette);
        PAL_setPalette(pal, black_palette, DMA);
    }

    VDP_drawImageEx(bg == BG_B_SELECT ? BG_B : BG_A, img,
                    TILE_ATTR_FULL(pal, false, 0, 0, ind),
                    0, 0, false, DMA);

    return ind + img->tileset->numTile;
}

u16 BACKGROUND_clear(BackgroundSelect bg)
{
    VDP_clearPlane(bg == BG_B_SELECT ? BG_B : BG_A, TRUE);

    UTILS_clear_palette(bg == BG_B_SELECT ? PAL_BACKGROUND_B : PAL_BACKGROUND_B);

    return TILE_USER_INDEX;
}

u16 BACKGROUND_full_clear(u16 ind)
{
    ind = BACKGROUND_clear(0);
    ind = BACKGROUND_clear(1);
    return ind;
}
