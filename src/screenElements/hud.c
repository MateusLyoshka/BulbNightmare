#include "hud.h"

// Declaration of GameObject structures for various HUD elements.
GameObject lives_sprite;
GameObject lives_text;
GameObject eye_sprite;
GameObject eye_text_sprite;
GameObject eye_text_sprite_2;
GameObject key_sprite;
GameObject key_text_sprite;
GameObject key_text_sprite_2;

u8 hud_initiated = 0;

// A constant array representing a solid black tile, used for the HUD background.
const u32 tile_black[8] =
    {
        0x11111111, // Represents 8x8 pixels, where '1' would typically be a color index.
        0x11111111,
        0x11111111,
        0x11111111,
        0x11111111,
        0x11111111,
        0x11111111,
        0x11111111};

/**
 * @brief Draws a black background for the HUD in the window plane.
 *
 * Loads a black tile into VRAM and then fills the window plane with it,
 * creating a solid black background for the HUD.
 *
 * @param ind The starting VRAM tile index to load the black tile data.
 * @return The next available VRAM tile index after the black tile.
 */
u16 HUD_background(u16 ind)
{
    VDP_loadTileData((const u32 *)tile_black, ind, 1, DMA);

    VDP_setWindowVPos(FALSE, HUD_TILES);
    // Fill the window tile map rectangle with the black tile.
    // TILE_ATTR_FULL sets the tile attributes: palette 0, priority 0, no flip, using the loaded tile index.
    // Fills from (0,0) to (39, HUD_TILES-1) (40 columns, HUD_TILES rows).
    VDP_fillTileMapRect(
        WINDOW,
        TILE_ATTR_FULL(PAL0, 0, 0, 0, ind),
        0, 0, 40, HUD_TILES);

    return ind + 1;
}

/**
 * @brief Initializes all HUD elements as GameObjects and sets their initial animations/values.
 *
 * If the HUD has been previously initiated, it first clears the existing HUD elements.
 *
 * @param ind The starting VRAM tile index for the sprites.
 * @return The next available VRAM tile index after all sprites have been initialized.
 */
u16 HUD_init(u16 ind)
{
    // If HUD has already been initiated, clear it first to re-initialize.
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

    // Set initial animations for the sprites.
    SPR_setAnim(lives_sprite.sprite, 2);
    SPR_setAnim(lives_text.sprite, player_lives); // Set lives text animation based on player_lives.

    SPR_setAnim(eye_sprite.sprite, 0);                                            // Set eye icon animation.
    SPR_setAnim(eye_text_sprite.sprite, 0);                                       // Set eye text 1 animation (likely a placeholder or for current count).
    SPR_setAnim(eye_text_sprite_2.sprite, switchs_on_level[LEVEL_current_level]); // Set eye text 2 animation based on total switches for current level.

    SPR_setAnim(key_sprite.sprite, 1);                                         // Set key icon animation.
    SPR_setAnim(key_text_sprite.sprite, player_keys);                          // Set key text 1 animation based on player_keys.
    SPR_setAnim(key_text_sprite_2.sprite, keys_on_level[LEVEL_current_level]); // Set key text 2 animation based on total keys for current level.

    hud_initiated = 1; // Mark HUD as initiated.

    return ind;
}

/**
 * @brief Updates the animation frames of HUD sprites to reflect current game stats.
 *
 * This function should be called regularly (e.g., in the game loop) to ensure
 * the HUD display is up-to-date.
 */
void HUD_update()
{
    // Update lives sprites.
    SPR_setAnim(lives_sprite.sprite, 2);
    SPR_setAnim(lives_text.sprite, player_lives);

    // Update eye (switch) sprites.
    SPR_setAnim(eye_sprite.sprite, 0);
    SPR_setAnim(eye_text_sprite.sprite, switchs_on);
    SPR_setAnim(eye_text_sprite_2.sprite, switchs_on_level[LEVEL_current_level]);

    // Update key sprites.
    SPR_setAnim(key_sprite.sprite, 1);
    SPR_setAnim(key_text_sprite.sprite, player_keys);
    SPR_setAnim(key_text_sprite_2.sprite, keys_on_level[LEVEL_current_level]);
}

/**
 * @brief Clears the HUD by releasing all associated sprites.
 *
 * This function releases the memory occupied by the HUD sprites and sets their
 * pointers to NULL to prevent dangling pointers.
 */
void HUD_clear()
{
    // Release sprites associated with lives.
    SPR_releaseSprite(lives_sprite.sprite);
    SPR_releaseSprite(lives_text.sprite);

    // Release sprites associated with eye (switches).
    SPR_releaseSprite(eye_sprite.sprite);
    SPR_releaseSprite(eye_text_sprite.sprite);
    SPR_releaseSprite(eye_text_sprite_2.sprite);

    // Release sprites associated with keys.
    SPR_releaseSprite(key_sprite.sprite);
    SPR_releaseSprite(key_text_sprite.sprite);
    SPR_releaseSprite(key_text_sprite_2.sprite);

    // Set sprite pointers to NULL to indicate they are no longer valid.
    lives_sprite.sprite = NULL;
    lives_text.sprite = NULL;

    eye_sprite.sprite = NULL;
    eye_text_sprite.sprite = NULL;
    eye_text_sprite_2.sprite = NULL;

    key_sprite.sprite = NULL;
    key_text_sprite.sprite = NULL;
    key_text_sprite_2.sprite = NULL;
}