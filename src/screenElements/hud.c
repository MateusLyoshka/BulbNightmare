#include "hud.h"

Sprite *hud_eye_sprite;
Sprite *hud_eye_text_sprite;
Sprite *hud_key_sprite;
Sprite *hud_key_text_sprite;

u16 HUD_init(u16 ind)
{
    HUD_clear();
    VDP_setTextPlane(WINDOW);
    VDP_setWindowVPos(FALSE, HUD_TILES);
    VDP_drawImageEx(WINDOW, &img_hud, TILE_ATTR_FULL(PAL_GAME, 1, 0, 0, ind), 0, 0, FALSE, DMA);
    ind += img_hud.tileset->numTile;

    hud_eye_sprite = SPR_addSprite(&spr_hud_itens, 32, 0, TILE_ATTR(PAL_GAME, 1, 0, 0));
    hud_eye_text_sprite = SPR_addSprite(&spr_hud_font, 48, 0, TILE_ATTR(PAL_GAME, 1, 0, 0));
    hud_key_sprite = SPR_addSprite(&spr_hud_itens, 80, 0, TILE_ATTR(PAL_GAME, 1, 0, 0));
    hud_key_text_sprite = SPR_addSprite(&spr_hud_font, 96, 0, TILE_ATTR(PAL_GAME, 1, 0, 0));

    SPR_setAnim(hud_eye_text_sprite, 1);
    SPR_setAnim(hud_key_sprite, 1);
    SPR_setAnim(hud_key_text_sprite, player_have_key);

    return ind;
}

void HUD_update()
{
    SPR_setAnim(hud_eye_text_sprite, 1);
    SPR_setAnim(hud_key_sprite, 1);
    SPR_setAnim(hud_key_text_sprite, player_have_key);
}

void HUD_clear()
{
    if (hud_eye_sprite)
        SPR_releaseSprite(hud_eye_sprite);
    if (hud_eye_text_sprite)
        SPR_releaseSprite(hud_eye_text_sprite);
    if (hud_key_sprite)
        SPR_releaseSprite(hud_key_sprite);
    if (hud_key_text_sprite)
        SPR_releaseSprite(hud_key_text_sprite);

    hud_eye_sprite = NULL;
    hud_eye_text_sprite = NULL;
    hud_key_sprite = NULL;
    hud_key_text_sprite = NULL;
}
