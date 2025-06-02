#include "background.h"

const Image *background_images[BG_MAX] = {
    [BG_LOGO] = &utf_logo,
    [BG_MENU] = &menu_bg,
    [BG_MENU_1] = &menu_bg_1,
    [BG_INSTRUCT] = &instructions,
};

u8 bg_color_delay = 5;
u8 bg_proceed = 0;

u16 BACKGROUND_init_generalized(BackgroundType type, u16 ind)
{
    BACKGROUND_clean(0);
    if (type >= BG_MAX || background_images[type] == NULL)
        return ind;

    const Image *img = background_images[type];

    PAL_setPalette(PAL_BACKGROUND_B, img->palette->data, DMA);

    VDP_drawImageEx(BG_B, img,
                    TILE_ATTR_FULL(PAL_BACKGROUND_B, 0, 0, 0, ind),
                    0, 0, FALSE, TRUE);

    return ind + img->tileset->numTile;
}

u16 BACKGROUND_clean(BackgroundSelect bg)
{
    VDP_clearPlane(bg == BG_B_SELECT ? BG_B : BG_A, TRUE);

    UTILS_clear_palette(bg == BG_B_SELECT ? PAL_BACKGROUND_B : PAL_BACKGROUND_B);

    return TILE_USER_INDEX;
}
