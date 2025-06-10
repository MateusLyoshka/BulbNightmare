#include "hud.h"

GameObject lives_sprite;
GameObject lives_text;
GameObject eye_sprite;
GameObject eye_text_sprite;
GameObject eye_text_sprite_2;
GameObject key_sprite;
GameObject key_text_sprite;
GameObject key_text_sprite_2;

u8 hud_initiated = 0;

const u32 tile_black[8] =
    {
        0x11111111,
        0x11111111,
        0x11111111,
        0x11111111,
        0x11111111,
        0x11111111,
        0x11111111,
        0x11111111};

u16 HUD_background(u16 ind)
{
    VDP_loadTileData((const u32 *)tile_black, ind, 1, DMA);

    VDP_setTextPlane(WINDOW);
    VDP_setWindowVPos(FALSE, HUD_TILES);
    VDP_fillTileMapRect(
        WINDOW,
        TILE_ATTR_FULL(PAL0, 0, 0, 0, ind),
        0, 0, 40, HUD_TILES);
    return ind + 1;
}

u16 HUD_init(u16 ind)
{
    if (hud_initiated)
    {
        HUD_clear();
    }
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
    SPR_setAnim(eye_text_sprite_2.sprite, switchs_on_level[LEVEL_current_level]);

    SPR_setAnim(key_sprite.sprite, 1);
    SPR_setAnim(key_text_sprite.sprite, player_keys);
    SPR_setAnim(key_text_sprite_2.sprite, keys_on_level[LEVEL_current_level]);

    hud_initiated = 1;

    return ind;
}

void HUD_update()
{
    SPR_setAnim(lives_sprite.sprite, 2);
    SPR_setAnim(lives_text.sprite, player_lives);

    SPR_setAnim(eye_sprite.sprite, 0);
    SPR_setAnim(eye_text_sprite.sprite, switchs_on);
    SPR_setAnim(eye_text_sprite_2.sprite, switchs_on_level[LEVEL_current_level]);

    SPR_setAnim(key_sprite.sprite, 1);
    SPR_setAnim(key_text_sprite.sprite, player_keys);
    SPR_setAnim(key_text_sprite_2.sprite, keys_on_level[LEVEL_current_level]);
}

void HUD_clear()
{
    SPR_releaseSprite(lives_sprite.sprite);
    SPR_releaseSprite(lives_text.sprite);

    SPR_releaseSprite(eye_sprite.sprite);
    SPR_releaseSprite(eye_text_sprite.sprite);
    SPR_releaseSprite(eye_text_sprite_2.sprite);

    SPR_releaseSprite(key_sprite.sprite);
    SPR_releaseSprite(key_text_sprite.sprite);
    SPR_releaseSprite(key_text_sprite_2.sprite);

    lives_sprite.sprite = NULL;
    lives_text.sprite = NULL;

    eye_sprite.sprite = NULL;
    eye_text_sprite.sprite = NULL;
    eye_text_sprite_2.sprite = NULL;

    key_sprite.sprite = NULL;
    key_text_sprite.sprite = NULL;
    key_text_sprite_2.sprite = NULL;
}
