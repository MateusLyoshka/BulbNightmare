#include "background.h"

u32 whiteTile[8] = {
    0x11111111,
    0x11111111,
    0x11111111,
    0x11111111,
    0x11111111,
    0x11111111,
    0x11111111,
    0x11111111,
};

u16 BACKGROUND_init(u16 ind)
{
    // Define o azul escuro (0x000057) na cor de índice 1 da PAL_BACKGROUND_B (BG_B)
    PAL_setColor(PAL0 * 16 + 1, RGB24_TO_VDPCOLOR(0x00000));

    // Carrega o tile com cor de índice 1
    VDP_loadTileData((const u32 *)whiteTile, ind, 1, DMA);

    // Preenche a tela inteira (40x28 tiles) no plano BG_B
    VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL0, 0, 0, 0, ind), 0, 0, 40, 28);

    return ind + 1;
}

u16 BACKGROUND_logo_init(u16 ind)
{
    PAL_setPalette(PAL1, utf_logo.palette->data, DMA);

    VDP_drawImageEx(BG_A, &utf_logo,
                    TILE_ATTR_FULL(PAL1, 0, 0, 0, ind),
                    0, 0, FALSE, TRUE);

    return ind;
}
