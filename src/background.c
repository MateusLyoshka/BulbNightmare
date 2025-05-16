#include "background.h"
#include "resources.h"
#include "globals.h"

// Associa numero de BGS a imagem
static const Image *backgrounds[BG_COUNT] = {
    &menu_bg,
    &white_bg};

u16 BACKGROUND_show(BackgroundType type, u16 tile_index)
{
    kprintf("Background type: %d\n", type);
    const Image *bg = backgrounds[type];

    VDP_setPlaneSize(64, 64, TRUE); // tamanho padrão da tela (320x224 px / 8 px)

    VDP_clearPlane(BG_B, TRUE);

    VDP_drawImageEx(
        BG_B,
        bg,
        TILE_ATTR_FULL(PAL_BACKGROUND_A, 0, 0, 0, tile_index),
        0, 0,
        TRUE,
        DMA);

    PAL_setPalette(PAL_BACKGROUND_A, bg->palette->data, DMA);

    return tile_index + bg->tileset->numTile;
}
