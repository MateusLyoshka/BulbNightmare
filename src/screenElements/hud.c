#include "hud.h"

GameObject lives_sprite;
GameObject lives_text;
GameObject eye_sprite;
GameObject eye_text_sprite;
GameObject eye_text_sprite_2;
GameObject key_sprite;
GameObject key_text_sprite;
GameObject key_text_sprite_2;

u16 HUD_init(u16 ind)
{
    HUD_clear();
    VDP_setTextPlane(WINDOW);
    VDP_setWindowVPos(FALSE, HUD_TILES);
    VDP_drawImageEx(WINDOW, &img_hud, TILE_ATTR_FULL(PAL_GAME, 1, 0, 0, ind), 0, 0, FALSE, DMA);
    ind += img_hud.tileset->numTile;

    ind += GAMEOBJECT_init(&lives_sprite, &spr_hud_itens, 16, 0, PAL_GAME, true, ind);
    ind += GAMEOBJECT_init(&lives_text, &spr_font_x, 32, 0, PAL_GAME, true, ind);

    ind += GAMEOBJECT_init(&eye_sprite, &spr_hud_itens, 64, 0, PAL_GAME, true, ind);
    ind += GAMEOBJECT_init(&eye_text_sprite, &spr_font_N, 80, 0, PAL_GAME, true, ind);
    ind += GAMEOBJECT_init(&eye_text_sprite_2, &spr_font_L, 96, 0, PAL_GAME, true, ind);

    ind += GAMEOBJECT_init(&key_sprite, &spr_hud_itens, 128, 0, PAL_GAME, true, ind);
    ind += GAMEOBJECT_init(&key_text_sprite, &spr_font_N, 144, 0, PAL_GAME, true, ind);
    ind += GAMEOBJECT_init(&key_text_sprite_2, &spr_font_L, 160, 0, PAL_GAME, true, ind);

    SPR_setAnim(lives_sprite.sprite, 2);
    SPR_setAnim(lives_text.sprite, player_lives);

    SPR_setAnim(eye_sprite.sprite, 0);
    SPR_setAnim(eye_text_sprite.sprite, 0);
    SPR_setAnim(eye_text_sprite_2.sprite, 3);

    SPR_setAnim(key_sprite.sprite, 1);
    SPR_setAnim(key_text_sprite.sprite, player_have_key);
    SPR_setAnim(key_text_sprite_2.sprite, 4);

    return ind;
}

void HUD_update()
{
    SPR_setAnim(lives_sprite.sprite, 2);
    SPR_setAnim(lives_text.sprite, player_lives);

    SPR_setAnim(eye_sprite.sprite, 0);
    SPR_setAnim(eye_text_sprite.sprite, 0);
    SPR_setAnim(eye_text_sprite_2.sprite, 3);

    SPR_setAnim(key_sprite.sprite, 1);
    SPR_setAnim(key_text_sprite.sprite, player_have_key);
    SPR_setAnim(key_text_sprite_2.sprite, 4);
    // SPR_setAnim(eye_text_sprite, 1);
    // SPR_setAnim(key_sprite, 1);
    // SPR_setAnim(key_text_sprite, player_have_key);
}

void HUD_clear()
{
    // SPR_releaseSprite(eye_sprite);
    // SPR_releaseSprite(eye_text_sprite);

    // SPR_releaseSprite(key_sprite);
    // SPR_releaseSprite(key_text_sprite);

    // eye_sprite = NULL;
    // eye_text_sprite = NULL;
    // key_sprite = NULL;
    // key_text_sprite = NULL;
}
