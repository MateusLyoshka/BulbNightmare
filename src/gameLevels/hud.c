#include "hud.h"

Sprite *hud_eye_sprite;
Sprite *hud_eye_text_sprite;
Sprite *hud_key_sprite;
Sprite *hud_key_text_sprite;

u16 HUD_init(u16 ind)
{
    VDP_setTextPlane(WINDOW);
    VDP_setWindowVPos(FALSE, HUD_TILES);
    VDP_drawImageEx(WINDOW, &img_hud, TILE_ATTR_FULL(PAL_BACKGROUND_B, 1, 0, 0, ind), 0, 0, FALSE, DMA);
    ind += img_hud.tileset->numTile;

    PAL_setPalette(PAL_PLAYER, spr_hud_itens.palette->data, DMA);
    PAL_setPalette(PAL_PLAYER, spr_hud_font.palette->data, DMA);

    hud_eye_sprite = SPR_addSprite(&spr_hud_itens, 32, 0, TILE_ATTR(PAL_PLAYER, 1, 0, 0));
    hud_eye_text_sprite = SPR_addSprite(&spr_hud_font, 48, 0, TILE_ATTR(PAL_PLAYER, 1, 0, 0));
    hud_key_sprite = SPR_addSprite(&spr_hud_itens, 80, 0, TILE_ATTR(PAL_PLAYER, 1, 0, 0));
    hud_key_text_sprite = SPR_addSprite(&spr_hud_font, 96, 0, TILE_ATTR(PAL_PLAYER, 1, 0, 0));

    SPR_setAnim(hud_eye_text_sprite, 1);
    SPR_setAnim(hud_key_sprite, 1);

    return ind;
}

void HUD_update(s16 light_power, s16 keys)
{
    SPR_setAnim(hud_eye_text_sprite, light_power);
    SPR_setAnim(hud_key_sprite, 1);
    SPR_setAnim(hud_key_text_sprite, keys);
}
