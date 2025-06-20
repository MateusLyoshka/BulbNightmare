#include "boss.h"

u16 random_timer = 0;
u8 timer_defined = 0;

GameObject boss;
GameObject face;
GameObject dialog;
GameObject boss_key;
GameObject boss_door;
GameObject enchant;

u16 boss_ind = TILE_USER_INDEX + 32;
u8 boss_proceed = 0;
u8 dialog_next = 0;
u8 transformed = 0;
u8 laughing = 0;
u8 room_black_bool = 0;

void BOSS_init()
{
    boss_ind = TILE_USER_INDEX + 32;
    boss_proceed = 0;
    dialog_next = 0;
    transformed = 0;
    laughing = 0;

    boss_ind = GAME_init();

    player_can_jump = 0;
    player_can_walk = 1;
    BOSS_flux_update(true);
}

void BOSS_flux()
{
    BOSS_init();

    while (!boss_proceed)
    {
        BOSS_flux_update(true);

        if (LEVEL_current_screen == 7)
        {
            if (!room_black_bool)
            {
                PAL_setPalette(PAL_BACKGROUND_B, black_palette, DMA);
                room_black_bool = 1;
            }

            if (player_center.tile_x == 4)
            {
                player_can_walk = 0;
                LEVEL_scroll_update_collision(640, 448);
                LEVEL_bool_screen_change = 0;
                LEVEL_bool_level_change = 0;
                for (u8 i = 0; i < 120; i++)
                {
                    SYS_doVBlankProcess();
                }
                BACKGROUND_clear(1);
                boss_proceed = 1;
            }
        }
    }

    boss_proceed = 0;
    VDP_setHorizontalScroll(BG_A, 0);
    VDP_setVerticalScroll(BG_A, 0);
    boss_ind = BACKGROUND_init_generalized(10, 1, PAL0, true, false, boss_ind);
    boss_ind = GAMEOBJECT_init(&face, &spr_face, 145, 42, PAL0, false, boss_ind);
    boss_ind = GAMEOBJECT_init(&boss, &spr_boss, 96, 0, PAL0, false, boss_ind);
    SPR_update();

    for (u8 i = 0; i < 60; i++)
    {
        BOSS_flux_update(false);
    }
    player_can_walk = 0;
    fadeIn(20, target_palette, black_palette, PAL0);
    waitMs(2000);
    BOSS_flux_update(false);
    waitMs(500);
    boss_ind = GAMEOBJECT_init(&dialog, &spr_dialog, 96, 168, PAL0, false, boss_ind);
    BOSS_speak_anim(false);

    while (!boss_proceed)
    {
        if (key_pressed(0, BUTTON_B))
        {
            dialog_next += 1;
            if (dialog_next == 12)
            {
                boss_proceed = 1;
            }
            else
            {
                SPR_setAnim(dialog.sprite, dialog_next);
            }
            if (dialog_next < 10)
            {
                BOSS_speak_anim(dialog_next > 8);
            }
        }
        if (dialog_next == 8 && !transformed)
        {
            BOSS_transform_anim();
            GAMEOBJECT_init(&enchant, &spr_enchant, player_center.x - 8, player_center.y - 8, PAL_GAME, false, boss_ind);
            for (u8 i = 0; i < 60; i++)
            {
                BOSS_flux_update(false);
            }
            SPR_releaseSprite(enchant.sprite);
            PLAYER_invert_gravity();
            BOSS_flux_update(false);
            while (!PLAYER_on_ground())
            {
                BOSS_flux_update(false);
            }
            GAMEOBJECT_init(&enchant, &spr_enchant, player_center.x - 8, player_center.y - 8, PAL_GAME, false, boss_ind);
            for (u8 i = 0; i < 60; i++)
            {
                BOSS_flux_update(false);
            }
            SPR_releaseSprite(enchant.sprite);
            PLAYER_invert_gravity();
            BOSS_flux_update(false);

            while (!PLAYER_on_ground())
            {
                BOSS_flux_update(false);
            }

            transformed = 1;
        }
        if (dialog_next == 10 && !laughing)
        {
            BOSS_laugh_anim();
            laughing = 1;
        }

        BOSS_flux_update(false);
    }
    BOSS_clear();

    OBJECT_update(boss_ind);
    player_can_walk = 1;
    player_can_jump = 1;
    while (!LEVEL_bool_level_change)
    {
        BOSS_flux_update(false);
    }

    return;
}

void BOSS_clear()
{
    BACKGROUND_clear(1);
    SPR_releaseSprite(boss.sprite);
    SPR_releaseSprite(face.sprite);
    SPR_releaseSprite(dialog.sprite);
    boss.sprite = NULL;
    face.sprite = NULL;
    dialog.sprite = NULL;
    SPR_update();
}

void BOSS_speak_anim()
{

    u16 random_timer = BOSS_random_crown_anim(90, 150, 30);

    SPR_setAnim(face.sprite, transformed ? 4 : 1);
    for (u8 i = 0; i < random_timer; i++)
    {
        BOSS_flux_update(false);
    }
    SPR_setAnim(boss.sprite, 0);
    SPR_setAnim(face.sprite, transformed ? 3 : 0);
}

void BOSS_transform_anim()
{
    SPR_setAnim(face.sprite, 2);
    for (u8 i = 0; i < 80; i++)
    {
        BOSS_flux_update(false);
    }
    SPR_setAnim(face.sprite, 3);
    return;
}

void BOSS_laugh_anim()
{
    SPR_setAnim(face.sprite, 5);
    for (u8 i = 0; i < 70; i++)
    {
        BOSS_flux_update(false);
    }
    SPR_setAnim(face.sprite, 6);
}

u16 BOSS_random_crown_anim(u16 min_val, u16 max_val, u16 step)
{
    s16 random_c = random() % 3;
    u16 timer_value = 0;
    if (!random_c)
    {
        timer_value = getRandomValueBetween(min_val, max_val, step);
    }
    if (random_c == 1)
    {
        SPR_setAnim(boss.sprite, 1);
    }

    return random_c ? 120 : timer_value;
}

void BOSS_flux_update(u8 mask_bool)
{
    update_input();
    PLAYER_update();
    if (mask_bool)
    {
        MASK_scroll_update();
    }

    LEVEL_update_camera(&player);
    SPR_update();
    SYS_doVBlankProcess();
}

void BOSS_power(u16 ind)
{
    if (!timer_defined || !player_is_alive)
    {
        random_timer = getRandomValueBetween(180, 300, 60);
        timer_defined = 1;
    }
    if (random_timer == 180)
    {
        GAMEOBJECT_init(&enchant, &spr_enchant, player_center.x - 8, player_center.y - 8, PAL_GAME, false, ind);
    }
    if (enchant.sprite != NULL)
    {

        s16 enchant_pos_x = player_center.x - (enchant.w / 2);
        s16 enchant_pos_y = player_center.y - (enchant.h / 2);

        SPR_setPosition(enchant.sprite, enchant_pos_x, enchant_pos_y);
    }
    if (random_timer == 0)
    {
        PLAYER_invert_gravity();
        timer_defined = 0;
        SPR_releaseSprite(enchant.sprite);
    }
    random_timer--;
}