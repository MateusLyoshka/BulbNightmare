#include "boss.h"

GameObject boss;
GameObject face;
GameObject dialog;

u16 boss_ind = TILE_USER_INDEX + 32;
u8 boss_proceed = 0;
u8 dialog_next = 0;

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
        if (player_center.tile_x == 4 && LEVEL_current_screen == 7)
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
    boss_ind = BACKGROUND_init_generalized(10, 1, PAL0, true, false, boss_ind);
    boss_ind = GAMEOBJECT_init(&face, &spr_face, 145, 42, PAL0, false, boss_ind);
    boss_ind = GAMEOBJECT_init(&boss, &spr_boss, 96, 0, PAL0, false, boss_ind);
    SPR_update();
    SPR_setAnim(face.sprite, 0);
    SPR_setAnim(boss.sprite, 0);
    for (u8 i = 0; i < 120; i++)
    {
        SYS_doVBlankProcess();
    }
    player_can_walk = 0;
    fadeIn(20, target_palette, black_palette, PAL0);
    waitMs(2000);
    BOSS_flux_update(false);
    waitMs(500);
    boss_ind = GAMEOBJECT_init(&dialog, &spr_dialog, 96, 168, PAL0, false, boss_ind);
    BOSS_speak_anim();
    while (!boss_proceed)
    {
        if (key_pressed(0, BUTTON_A))
        {
            dialog_next += 1;
            SPR_setAnim(dialog.sprite, dialog_next);
            BOSS_speak_anim();
        }

        BOSS_flux_update(false);
    }
}

void BOSS_speak_anim()
{
    u16 randomSpeak = getRandomValue_90_180_step30();
    kprintf("%d", randomSpeak);
    SPR_setAnim(face.sprite, 1);
    for (u8 i = 0; i < randomSpeak; i++)
    {
        BOSS_flux_update(false);
    }
    SPR_setAnim(face.sprite, 0);
    BOSS_flux_update(false);
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