#include "game.h"

u16 ind = TILE_USER_INDEX + 32;
u8 game_initiated;
u8 game_ended = 0;

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

void GAME_init_sounds()
{
    XGM_setPCM(65, snd_start, sizeof(snd_start));
    XGM_setPCM(66, snd_death, sizeof(snd_death));
    XGM_setPCM(67, snd_key, sizeof(snd_key));
    XGM_setPCM(68, snd_switch, sizeof(snd_switch));
    XGM_setPCM(69, snd_jump, sizeof(snd_jump));
    XGM_setPCM(70, snd_button_change, sizeof(snd_button_change));
    XGM_setPCM(71, snd_boss_power, sizeof(snd_boss_power));
    XGM_setPCM(72, snd_door, sizeof(snd_door));
}

void GAME_update()
{
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
    if (LEVEL_current_level == 5)
    {
        BOSS_power(ind);
    }

    GAME_pause_init();
    MASK_scroll_update();
    update_input();
    PLAYER_update();
    LEVEL_update_camera(&player);
    ENEMIES_update_hub();
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
    if (LEVEL_current_level < 4)
    {
        ind = HUD_background(ind);
        HUD_init(ind);
    }
    else if (LEVEL_current_level == 4)
    {
        HUD_clear();
        VDP_setWindowVPos(FALSE, 0);
    }
    ind = LEVEL_init(ind);
    ind = PLAYER_init(ind);
    ind = ENEMIES_spawn_hub(ind);

    GAME_mask_init();

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
    if (LEVEL_current_level < 5)
    {
        player_lives -= 1;
    }

    if (!player_lives)
    {
        game_initiated = 0;
        GAME_level_change();
    }
    else
    {
        PLAYER_death_anim();
        LEVEL_scroll_update_collision(0, 448);
        GAME_screen_change();
        PLAYER_respawn();
        player_is_alive = 1;
    }
}

void GAME_end_game()
{
    set_black_palletes();
    ind = BACKGROUND_init_generalized(11, 1, PAL0, true, true, ind);
    fadeIn(60, target_palette, black_palette, PAL0);
    waitMs(1000);
    fadeOut(60, PAL0);
    ind = BACKGROUND_init_generalized(12, 1, PAL0, true, true, ind);
    fadeIn(60, target_palette, black_palette, PAL0);
    waitMs(1000);
    fadeOut(60, PAL0);
    ind = BACKGROUND_init_generalized(13, 1, PAL0, true, true, ind);
    fadeIn(60, target_palette, black_palette, PAL0);
    waitMs(1000);
    fadeOut(60, PAL0);
    ind = BACKGROUND_init_generalized(14, 1, PAL0, true, true, ind);
    fadeIn(120, target_palette, black_palette, PAL0);
    waitMs(4000);
    fadeOut(120, PAL0);
    ind = BACKGROUND_clear(1);
    game_ended = 1;
    LEVEL_bool_level_change = 0;
    GAME_reset();
}

void GAME_level_change()
{
    ENEMIES_level_change_despawn();
    if (player_lives && !pause_proceed)
    {
        LEVEL_current_level += 1;
    }
    else
    {
        LEVEL_current_level = 0;
    }
    if (LEVEL_current_level == 6)
    {
        kprintf("level: %d", LEVEL_current_level);
        GAME_end_game();
        return;
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
    if (player_is_alive || (!player_is_alive && LEVEL_last_screen != 6))
    {

        ind = ENEMIES_spawn_hub(ind);
    }
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

    if (LEVEL_current_level < 5)
    {
        HUD_clear();
    }

    if (enchant.sprite != NULL)
    {
        SPR_releaseSprite(enchant.sprite);
        enchant.sprite = NULL;
    }
    LEVEL_current_level = 0;
    ind = BACKGROUND_clear(0);
    ind = BACKGROUND_clear(1);
    ind = BACKGROUND_clear(2);
    OBJECT_update(ind);
    ENEMIES_update_hub();
    OBJECT_clear_all();
    ENEMIES_level_change_despawn();
    VDP_setWindowVPos(FALSE, 0);
    PLAYER_free();
    SPR_update();
    SYS_doVBlankProcess();
    GAME_menu_init();
    GAME_init();
}
