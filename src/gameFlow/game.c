#include "game.h"

u16 ind = TILE_USER_INDEX + 32;
u8 game_initiated;

const u16 *game_palettes[6];

void GAME_init_palettes()
{
    game_palettes[0] = level1_pal.data;
    game_palettes[1] = level2_pal.data;
    game_palettes[2] = level3_pal.data;
    game_palettes[3] = level4_pal.data;
    game_palettes[4] = level4_pal.data;
    game_palettes[5] = boss_pal.data;
}

void GAME_update()
{
    GAME_pause_init();
    if (!player_is_alive)
    {
        GAME_player_death();
    }
    if (LEVEL_bool_screen_change)
    {
        GAME_screen_change();
    }
    if (LEVEL_bool_level_change)
    {
        GAME_level_change();
    }

    MASK_scroll_update();
    update_input();
    PLAYER_update();
    LEVEL_update_camera(&player);
    // ENEMIES_update_hub(enemies_current_level, enemies_past_level);
    HUD_update();
}

u16 GAME_init()
{
    set_black_palletes();
    if (!game_initiated)
    {
        OBJECT_key_reset();
    }
    ind = LEVEL_alert(ind);
    ind = HUD_background(ind);
    // ENEMIES_init();
    ind = LEVEL_init(ind);
    ind = PLAYER_init(ind);
    GAME_mask_init();

    // ind = ENEMIES_spawn_hub(enemies_current_level, enemies_past_level, ind);
    HUD_init(ind);
    OBJECT_update(ind);
    LEVEL_update_camera(&player);
    PLAYER_respawn();
    game_initiated = 1;
    SPR_update();
    SYS_doVBlankProcess();
    GAME_update_palletes();
    return ind;
}

void GAME_update_palletes()
{
    PAL_setPalette(PAL_GAME, game_pal.data, DMA);
    PAL_setPalette(PAL_BACKGROUND_B, game_palettes[LEVEL_current_level], DMA);
}

void GAME_mask_init()
{
    MASK_scroll_init();
    MASK_draw(dark_ind);
    MASK_scroll_update();
    SPR_update();
}

void GAME_player_death()
{
    player_lives -= 1;
    if (!player_lives)
    {
        game_initiated = 0;
        GAME_level_change();
    }
    else
    {
        LEVEL_scroll_update_collision(0, 448);
        GAME_screen_change();
        PLAYER_respawn();
        player_is_alive = 1;
    }
}

void GAME_level_change()
{
    // ENEMIES_level_change_despawn(enemies_current_level, enemies_past_level);
    if (player_lives && !pause_proceed)
    {
        LEVEL_current_level += 1;
    }
    else
    {
        LEVEL_current_level = 0;
    }
    ind = TILE_USER_INDEX + 32;
    player_keys = 0;
    switchs_on = 0;
    player_is_alive = 1;
    LEVEL_bool_level_change = 0;
    if (LEVEL_current_level == 4)
    {
        BOSS_flux();
    }
    else
    {
        GAME_init();
    }
}

void GAME_screen_change()
{

    // ind = ENEMIES_spawn_hub(enemies_current_level, enemies_past_level, ind);
    HUD_init(ind);
    OBJECT_update(ind);
    MASK_draw();
    LEVEL_bool_screen_change = 0;
}

void GAME_menu_init()
{
    ind = BACKGROUND_init_generalized(6, 1, PAL0, true, true, ind);
    fadeIn(60, target_palette, black_palette, PAL0);
    waitMs(1000);
    fadeOut(60, PAL0);
    ind = BACKGROUND_clear(1);

    ind = BACKGROUND_init_generalized(7, 1, PAL0, true, true, ind);

    fadeIn(60, target_palette, black_palette, PAL0);
    ind = MENU_init(ind);

    while (!bg_proceed)
    {
        ind = MENU_update(ind);
        update_input();
        SPR_update();
        SYS_doVBlankProcess();
    }
    ind = TILE_USER_INDEX + 32;
}

void GAME_pause_init()
{
    if (key_pressed(0, BUTTON_START))
    {
        ind = PAUSE_init(ind);
        while (!pause_proceed)
        {
            PAUSE_update();
            update_input();
            SPR_update();
            SYS_doVBlankProcess();
        }
        PAUSE_clear();
        SPR_update();
        if (pause_proceed == 1)
        {
            ind = TILE_USER_INDEX + 32;
            ind = HUD_background(ind);
            ind = LEVEL_init(ind);
            MASK_scroll_init();
            MASK_draw(dark_ind);
        }
        else if (pause_proceed == 2)
        {
            GAME_level_change();
        }
        else if (pause_proceed == 3)
        {
            GAME_reset();
        }
        pause_proceed = 0;
    }
}

void GAME_reset()
{
    player_keys = 0;
    switchs_on = 0;
    game_initiated = 0;
    hud_initiated = 0;
    LEVEL_current_level = 0;
    ind = BACKGROUND_clear(0);
    ind = BACKGROUND_clear(1);
    ind = BACKGROUND_clear(2);
    VDP_setWindowVPos(FALSE, 0);
    PLAYER_free();
    OBJECT_clear_all();
    HUD_clear();
    SPR_update();
    SYS_doVBlankProcess();
    GAME_menu_init();
    GAME_init();
}
