#include "menu.h"

GameObject start;
GameObject menu;

u8 room = 0;
u8 current_room = 0;
u8 last_room = 0;
s8 menu_option = -1;

u16 MENU_init(u16 ind)
{
    ind += GAMEOBJECT_init(&start, &spr_start, 97, 86, PAL_GAME, ind);
    // kprintf("a");
    return ind;
}

u16 MENU_update(u16 ind)
{
    if (key_pressed(0, BUTTON_START) && menu_option == -1)
    {
        menu_option++;
        SPR_releaseSprite(start.sprite);
        ind += GAMEOBJECT_init(&menu, &spr_menu, 96, 72, PAL_GAME, ind);
    }
    else if (key_pressed(0, BUTTON_DOWN) && menu_option == 0)
    {
        menu_option++;
        menu.anim = menu_option;
        SPR_setAnim(menu.sprite, menu_option);
    }
    else if (key_pressed(0, BUTTON_DOWN) && menu_option == 1)
    {
        menu_option++;
        menu.anim = menu_option;
        SPR_setAnim(menu.sprite, menu_option);
    }
    else if (key_pressed(0, BUTTON_UP) && menu_option == 2)
    {
        menu_option--;
        menu.anim = menu_option;
        SPR_setAnim(menu.sprite, menu_option);
    }
    else if (key_pressed(0, BUTTON_UP) && menu_option == 1)
    {
        menu_option--;
        menu.anim = menu_option;
        SPR_setAnim(menu.sprite, menu_option);
    }
    else if (key_pressed(0, BUTTON_A) && menu_option == 0)
    {
        ind = BACKGROUND_clean(0);
        ind = BACKGROUND_clean(1);
        SPR_releaseSprite(menu.sprite);
        bg_proceed = 1;
    }
    else if (key_pressed(0, BUTTON_A) && menu_option == 1 && current_room == 0)
    {
        current_room = 1;
        ind = BACKGROUND_init_generalized(3, 0, PAL_BACKGROUND_A, ind);
        SPR_releaseSprite(menu.sprite);
    }
    else if (key_pressed(0, BUTTON_A) && current_room == 1)
    {
        current_room = 0;
        ind = BACKGROUND_init_generalized(2, 1, PAL_BACKGROUND_B, ind);
        ind = GAMEOBJECT_init(&menu, &spr_menu, 96, 72, PAL_GAME, ind);
        SPR_setAnim(menu.sprite, menu_option);
    }
    // kprintf("%d", ind);
    return ind;
}