#include "menu.h"

GameObject start;
GameObject menu;

u8 room = 0;
u8 current_room = 0;
s8 menu_option = -1;

u16 MENU_init(u16 ind)
{
    ind += GAMEOBJECT_init(&start, &spr_start, 97, 86, PAL_BACKGROUND_A, ind);
    kprintf("a");
    return ind;
}

u16 MENU_update(u16 ind)
{
    if (key_pressed(0, BUTTON_START) && menu_option == -1)
    {
        menu_option++;
        SPR_releaseSprite(start.sprite);
        ind += GAMEOBJECT_init(&menu, &spr_menu, 96, 72, PAL_BACKGROUND_A, ind);
    }
    return ind;
}