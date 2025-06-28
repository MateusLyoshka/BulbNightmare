#include "menu.h"

GameObject start;
GameObject menu;

u8 room = 0;
u8 current_room = 0;
s8 menu_option = -1;

u16 MENU_init(u16 ind)
{
    room = 0;
    current_room = 0;
    menu_option = -1;
    bg_proceed = 0;
    PAL_setPalette(PAL_BACKGROUND_A, spr_start.palette->data, DMA);
    ind += GAMEOBJECT_init(&start, &spr_start, 97, 86, PAL_BACKGROUND_A, true, ind);
    return ind;
}

u16 MENU_update(u16 ind)
{
    if (key_pressed(0, BUTTON_START) && menu_option == -1)
    {
        menu_option++;
        SPR_releaseSprite(start.sprite);
        start.sprite = NULL;
        ind += GAMEOBJECT_init(&menu, &spr_menu, 96, 72, PAL_BACKGROUND_A, true, ind);

        XGM_startPlayPCM(65, 1, SOUND_PCM_CH1);
    }
    else if (key_pressed(0, BUTTON_DOWN) && menu_option == 0)
    {
        XGM_startPlayPCM(70, 1, SOUND_PCM_CH1);
        menu_option++;
        SPR_setAnim(menu.sprite, menu_option);
    }
    else if (key_pressed(0, BUTTON_DOWN) && menu_option == 1)
    {
        XGM_startPlayPCM(70, 1, SOUND_PCM_CH1);
        menu_option++;
        SPR_setAnim(menu.sprite, menu_option);
    }
    else if (key_pressed(0, BUTTON_UP) && menu_option == 2)
    {
        XGM_startPlayPCM(70, 1, SOUND_PCM_CH1);
        menu_option--;
        SPR_setAnim(menu.sprite, menu_option);
    }
    else if (key_pressed(0, BUTTON_UP) && menu_option == 1)
    {
        XGM_startPlayPCM(70, 1, SOUND_PCM_CH1);
        menu_option--;
        SPR_setAnim(menu.sprite, menu_option);
    }
    else if (key_pressed(0, BUTTON_A) && menu_option == 0)
    {
        XGM_startPlayPCM(65, 1, SOUND_PCM_CH1);
        SPR_releaseSprite(menu.sprite);
        SPR_update();
        fadeOut(60, PAL0);
        waitMs(400);
        ind = BACKGROUND_clear(1);
        bg_proceed = 1;
    }
    else if (key_pressed(0, BUTTON_A) && menu_option == 1 && current_room == 0)
    {
        XGM_startPlayPCM(65, 1, SOUND_PCM_CH1);
        current_room = 1;
        ind = BACKGROUND_init_generalized(8, 1, PAL_BACKGROUND_B, false, false, ind);
        SPR_releaseSprite(menu.sprite);
    }
    else if (key_pressed(0, BUTTON_A) && current_room == 1)
    {
        current_room = 0;
        ind = BACKGROUND_init_generalized(7, 1, PAL_BACKGROUND_B, false, true, ind);
        ind = GAMEOBJECT_init(&menu, &spr_menu, 96, 72, PAL_BACKGROUND_A, true, ind);
        SPR_setAnim(menu.sprite, menu_option);
    }

    return ind;
}