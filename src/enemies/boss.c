#include "boss.h"

u16 boss_ind = TILE_USER_INDEX + 32;
u8 boss_proceed = 0;

u16 BOSS_init(u16 ind)
{
    return ind;
}

void BOSS_flux()
{
    boss_ind = LEVEL_init(boss_ind);
    boss_ind = PLAYER_init(boss_ind);
    GAME_mask_init();
    LEVEL_update_camera(&player);
    SPR_update();
    SYS_doVBlankProcess();
    PLAYER_respawn();
    kprintf("%d", boss_ind);
    player_can_jump = 0;
    while (!boss_proceed)
    {
        BOSS_flux_update(true);
        if (player_center.tile_x == 5 && LEVEL_current_screen == 7)
        {
            player_can_walk = 0;
            LEVEL_scroll_update_collision(640, 448);
            for (u8 i = 0; i < 120; i++)
            {
                SYS_doVBlankProcess();
            }
            BACKGROUND_clear(1);
            boss_proceed = 1;
        }
    }
    kprintf("%d", boss_ind);
    boss_proceed = 0;
    VDP_setHorizontalScroll(BG_A, 0);
    VDP_setVerticalScroll(BG_A, 0);
    boss_ind = BACKGROUND_init_generalized(10, 1, PAL0, true, false, boss_ind + 64);
    for (u8 i = 0; i < 120; i++)
    {
        SYS_doVBlankProcess();
    }
    player_can_walk = 1;
    fadeIn(20, target_palette, black_palette, PAL0);
    waitMs(2000);
    while (!boss_proceed)
    {
        BOSS_flux_update(false);
    }
}

void BOSS_flux_update(u8 mask_bool)
{
    if (mask_bool)
    {
        MASK_scroll_update();
    }

    update_input();
    PLAYER_update();
    LEVEL_update_camera(&player);
    SPR_update();
    SYS_doVBlankProcess();
}